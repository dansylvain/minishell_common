/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dan <dan@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 08:59:03 by dan               #+#    #+#             */
/*   Updated: 2024/02/19 14:10:57 by dan              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <sys/wait.h>
# include <sys/types.h>
# include <signal.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "../lib/ft_printf/ft_printf.h"
# include "../lib/libft/libft.h"
// # include "../lib/get_next_line/get_next_line.h"
# include <readline/readline.h>
# include <readline/history.h>
# include "parsing_utils.h"

typedef struct t_Data
{
	char	**envp_tab;
	int		exit_status;
}	t_Data;

void	handle_signals(void);
int		prompt_loop(t_Data *data, char *envp[]);
void	display_error(char *str);
void	free_data(t_Data *data);
void	close_minishell(t_Data *data);
void	exec_env(t_Data *data, char **command_tab);
int		command_is_builtin(char	*cmd[], t_Data *data, char *envp[]);
void	exec_pwd(t_Data *data);
int		exec_cd(t_Data *data, char **command_tab);
void	exec_echo(t_Data *data, char **command_tab);
void	exec_unset(t_Data *data, char **command_tab);
void	free_command_tab(char **command_tab);
char	**duplicate_envp(t_Data *data, char *envp[]);
int		exec_export(char **command_tab, t_Data *data);

/*   exec_export.c                                  :+:      :+:    :+:   */
void	add_env_var_to_envp_tab(char **command_tab, t_Data *data, int *i);
char	**create_new_env_var(char **envp, char *env_var);
char	*get_env_var(t_Data *data, char *to_find);
void	display_export_tab(char export_tab[][500]);
void	create_export_tab(t_Data *data, char export_tab[][500]);

/*   exec_echo.c                                  :+:      :+:    :+:   */
int	is_only_space(char *str);

/*   exec_exit.c                                  :+:      :+:    :+:   */
int	str_is_digit(char *str);

/*   exec_unset.c                                  :+:      :+:    :+:   */
void	unset_var(char **envp, int index);


void		free_command_tab_lg(char ***command_tab);
int			pipex(char **argv[], char *envp[]);
t_ast_nde	*copy_node_child(t_ast_nde *node);

#endif
