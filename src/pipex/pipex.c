/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seblin <seblin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:23:23 by svidot            #+#    #+#             */
/*   Updated: 2024/02/23 11:25:01 by seblin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "ft_printf.h"
#include "libft.h"
#include "../../lib/get_next_line/get_next_line.h"
#include "pipex_setup.h"
#include "../../includes/minishell.h"

void	close_fd(int fd[]);
// void	set_filepath_and_delim(int *argc, char **argv[], t_redir *redir);
int	get_fdio(t_redir *redir);
//char	**parse_cmd(char *argv[], char *envp[]);
int	command_is_builtin(char	*cmd[], t_Data *data, char *envp[]);
char	**search_path(char *argv[], char *envp[]);

void	set_pipe_forward(int pipefd_in[], int pipefd_out[], t_redir redir)
{
	if (pipefd_in[0])
	{		
		dup2(pipefd_in[0], STDIN_FILENO);
		close(pipefd_in[0]);
	}
		close(pipefd_in[1]);
	dup2(pipefd_out[1], STDOUT_FILENO);
	close(pipefd_out[1]);
	close(pipefd_out[0]);
}

pid_t	nurcery(char **argv[], char *envp[], int fd_file[], int *pipefd[], t_redir redir)
{
	pid_t	pid;

	while (*argv)
	{
		if (***argv != '>' && ***argv != '<')
		{			
			pid = fork();
			if (pid == 0)
			{
				if (fd_file[1] > 2)
					close(fd_file[1]);
				set_pipe_forward(pipefd[0], pipefd[1], redir);
				//ft_printf("argv in nurcery -%s-%s\n", **argv, (*argv)[1]);
				if (!command_is_builtin(*argv, NULL, envp))
				{
					if (access(**argv, X_OK))
						search_path(*argv, envp);				
					execve(**argv, *argv, envp);
					exit(EXIT_FAILURE);
				}
				else
				{					
					exit(EXIT_SUCCESS);	
				}
			}
			else
			{
				close(pipefd[0][1]);
				if (pipefd[0][0] > 2)
					close(pipefd[0][0]);
				pipefd[0][0] = pipefd[1][0];
				pipefd[0][1] = pipefd[1][1];
				pipe(pipefd[1]); 
			}
		}
		argv++;
	}
	return (pid);
}

void	here_doc_handle(int pipefd_in[], t_redir redir) //argv!!
{
	char	*line;
		
	while (1)
	{
		ft_printf("heredoc> ");
		line = get_next_line(0);
		if (line)
		{
			line[ft_strlen(line) - 1] = 0; //optimisable ... ajout \n a delim
			if (ft_strcmp(line, redir.delim))
			{
				line[ft_strlen(line)] = '\n';
				ft_putstr_fd(line, pipefd_in[1]);
			}
			else
			{
				free(line);
				get_next_line(42);
				break ;
			}
		}
		else
		{
			ft_printf("\rwarning: here-document at line 1 delimited by end-of-file (wanted '%s')\n", redir.delim);
			break ;
		}
		free(line); //opti
	}
}
// void	here_doc_handle(char **argv[], int pipefd_in[])
// {
// 	(void) argv;
// 	(void) pipefd_in;
// }
pid_t	create_pipeline(char **argv[], char *envp[], t_redir redir)
{
	int		pipefd_in[2];
	int		pipefd_out[2];
	char	buf;
	pid_t	pid;
	
	pid = -1;
	if (redir.pipe[0] < 0)
		pipe(pipefd_in);
	else
	{
		pipefd_in[0] = redir.pipe[0];
		pipefd_in[1] = redir.pipe[1];
	}
	pipe(pipefd_out); // optim ?
	if (!redir.redir[0])
	{
		close(pipefd_in[0]);
		pipefd_in[0] = 0;
	}
	else if (redir.redir[0] == 1)
	{		
		close(pipefd_in[0]);
		pipefd_in[0] = redir.fd_file[0];
	}
	// else if (redir.redir[0] == 2)
	// 	redir.pipe[0] = 
	// 	here_doc_handle(pipefd_in, redir);
	pid = nurcery(argv, envp, redir.fd_file, (int *[]){pipefd_in, pipefd_out}, redir);
	close(pipefd_in[1]);
	close(pipefd_out[1]);
	close(pipefd_out[0]);
	if (redir.redir[1])
	{
		while (read(pipefd_in[0], &buf, 1))
			ft_putchar_fd(buf, redir.fd_file[1]);
		close(redir.fd_file[1]);		
	}
	else
	{		
		while (read(pipefd_in[0], &buf, 1))
			ft_putchar_fd(buf, 1);		
	}
	close(pipefd_in[0]);
	return (pid);
}

int	arr_len(const void *arr[])
{
	int	i;

	i = 0;
	while(arr[i])
		i++;
	return (i);
}

void	set_redir2(int argc, char **argv[], int redir[])
{	
	redir[0] = 0;
	redir[1] = 0;
	if (***argv == '<')
	{
		redir[0] = 1;
		if (!ft_strcmp(**argv, "<<"))
			redir[0] = 2;	 			
	}
	if (**(argv)[argc - 1] == '>')
	{
		redir[1] = 1;		
		if (!ft_strcmp(*argv[argc - 1], ">>"))
			redir[1] = 2;	
	}
}
void	set_filepath_and_delim2(int *argc, char ***argv[], t_redir *redir)
{
	redir->delim = NULL;
	redir->filepath[0] = NULL;
	redir->filepath[1] = NULL;
	if (redir->redir[0] == 1)	
		redir->filepath[0] = (*(*argv)++)[1];
	else if (redir->redir[0] == 2)
		redir->delim = (*(*argv)++)[1];
	if (redir->redir[0])
		(*argc)--;
	if (redir->redir[1])
	redir->filepath[1] = (*argv)[(*argc) - 1][1];
}
// void	set_filepath(int *argc, char **argv, char *filepath[])
// {
// 	if (redir->redir[0] == 1)	
// 		redir->filepath[0] = (*argv)++[1];	
// 	if (redir->redir[0])
// 		(*argc)--;
// 	if (redir->redir[1])
// 	redir->filepath[1] = (*argv)[(*argc) - 1][1];

// }
int	set_redir(char **argv, t_redir *redir)
{
	if (**argv == '<')
	{
		redir->redir[0] = 1;
		if (!ft_strcmp(*argv, "<<"))
		{
			redir->delim = argv[1];
			redir->redir[0] = 2;
			close_fd(redir->pipe);
			pipe(redir->pipe);
			here_doc_handle(redir->pipe, *redir);		
		}
		else
		{
			redir->filepath[0] = argv[1];
			if (get_fdio(redir))
				return (1);			
		}			
	}
	else if (**argv == '>')
	{
		redir->redir[1] = 1;		
		if (!ft_strcmp(*argv, ">>"))
			redir->redir[1] = 2;
		redir->filepath[1] = argv[1];
		if (get_fdio(redir))
			return (1);
	}
	return (0);
}
int	set_io(char **argv[], t_redir *redir)
{
	redir->fd_file[0] = -1;
	redir->fd_file[1] = -1;
	redir->pipe[0] = -1;
	redir->pipe[1] = -1;
	redir->redir[0] = 0;	
	redir->redir[1] = 0;
	redir->delim = NULL;
	redir->filepath[0] = NULL;
	redir->filepath[1] = NULL;
	while (*argv)
	{
		if (set_redir(*argv, redir))
			return (1);
		argv++;
	}
	return (0);
}

int	pipex(char **argv[], char *envp[])
{
	t_redir redir;
	int		argc;
	pid_t	pid;
	int status;
	int exit_status;
	//ft_printf("argv in nurcery");// -%s-%s\n", **argv, (*argv)[1]);
	exit_status = -1;
	pid = -1;//ft_printf("argv: %s, redir 0:%d, redir 1:%d, fdfile 0:%d, fdfile 1:%d, filepath 0:%s, filepath 1:%s, delim : %s\n", **argv, redir.redir[0], redir.redir[1], redir.fd_file[0], redir.fd_file[1], redir.filepath[0], redir.filepath[1], redir.delim); 

	argc = arr_len((void *)argv);//ft_printf("argc %d\n", argc);
	if (!argc)
		return (1);
	
	// set_redir(argc, argv, redir.redir);
	// set_filepath_and_delim(&argc, &argv, &redir);
	// get_fdio(&redir);
	if (set_io(argv, &redir))//
		return (1);
	pid = create_pipeline(argv, envp, redir);
	// ft_printf("argv: %s, redir 0:%d, redir 1:%d, fdfile 0:%d, fdfile 1:%d, filepath 0:%s, filepath 1:%s, delim : %s\n", **argv, redir.redir[0], redir.redir[1], redir.fd_file[0], redir.fd_file[1], redir.filepath[0], redir.filepath[1], redir.delim); //exit(1);
	
	if(waitpid(pid, &status, 0) > 0) // Attend spécifiquement la fin du dernier processus enfant
	{
		if (WIFEXITED(status))		
    		exit_status = WEXITSTATUS(status);	
		if (exit_status >= 1)
			exit_status = 127;
	}
	else 
    	;//ft_printf("waitpid a échoué ou il n'y avait aucun enfant à attendre\n");
	while (wait(&(int){0}) > 0)
		;
	return (exit_status);
}

#pragma GCC diagnostic pop
