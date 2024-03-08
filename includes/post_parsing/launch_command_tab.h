/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_command_tab.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dan <dan@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 19:59:07 by dan               #+#    #+#             */
/*   Updated: 2024/03/08 20:26:59 by dan              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LAUNCH_COMMAND_TAB_H
# define LAUNCH_COMMAND_TAB_H

# include "minishell.h"
# include "parsing_utils.h"
# include <stddef.h>

void	store_and_free_cmd_tab_node_sav(t_ast_nde *cmd_tab_node_sav);
int		is_pipeline(t_ast_nde *cmd_tab_node_sav);
int		pipex(char **argv[]);
int		command_is_builtin(char	**cmd_tab, t_Data *data);
void	free_command_tab_lg(char ***command_tab);
char	***create_command_tab(t_Data *data, t_ast_nde *node, char *envp[]);
void	build_command_tab(char ****cmd_tab, t_Data *data,
			t_ast_nde **cmd_tab_node_sav, char *envp[]);
void	store_and_free_cmd_tab(char ***cmd_tab);

#endif
