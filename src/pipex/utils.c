/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seblin <seblin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 17:13:27 by seblin            #+#    #+#             */
/*   Updated: 2024/03/08 18:36:09 by seblin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_utils.h"

void	free_all(char **argv_sav[])
{
	free_data(get_data(NULL));
	free_command_tab_lg(argv_sav);
	store_and_free_cmd_list(NULL);
}

void	free_and_exit(char **argv_sav[], int n_exit, char *err_str)
{
	if (err_str)
		display_error(err_str);
	free_all(argv_sav);
	exit(n_exit);
}

int	is_dir(char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == 0)
	{
		if (S_ISREG(path_stat.st_mode))
			return (0);
		else if (S_ISDIR(path_stat.st_mode))
			return (1);
		else
			return (0);
	}
	return (0);
}

void	check_filedir_error(char **argv[], char **argv_sav[])
{
	if (!access(**argv, F_OK))
	{
		if (***argv == '.' && argv[0][0][1] == '/' || argv[0][0][0] == '/')
		{
			if (access(**argv, X_OK))
				free_and_exit(argv_sav, 126, " Permission denied\n");
		}
		else
			free_and_exit(argv_sav, 127, " command not found\n");
	}
	else if (***argv == '.' && argv[0][0][1] == '/' || argv[0][0][0] == '/')
		display_error(" No such file or directory\n");
}

void	here_doc_handle(int pipefd_in[], t_redir redir)
{
	char	*line;

	while (1) //argv!!
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
			ft_printf("\rwarning: here-document at line 1 \
delimited by end-of-file (wanted '%s')\n", redir.delim);
			break ;
		}
		free(line); //opti
	}
}