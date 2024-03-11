/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seblin <seblin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 19:33:40 by dan               #+#    #+#             */
/*   Updated: 2024/03/11 17:08:39 by seblin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_pipex.h"

#include <stdio.h>
/**========================================================================
 *                           exec_pipex
 *========================================================================**/
int	exec_pipex(t_Data *data, char *cmd, char *envp[], int reset)
{
	t_ast_nde	*cmd_list;
	static int	or_flag;
	if (reset)
	{
		or_flag = 0;
		return (0);
	}
	//printf("orflag: %i\n", or_flag);
	cmd_list = NULL;
	cmd_list = parse(cmd, data);
	if (!cmd_list)
		return (-1) ;
	store_and_free_cmd_list(cmd_list);
	or_flag = launch_command_tab(data, cmd_list, envp, or_flag);
	store_and_free_cmd_list(NULL);
	return (or_flag);
}

/**========================================================================
 *                           store_and_free_cmd_list
 *========================================================================**/
void	store_and_free_cmd_list(t_ast_nde *cmd_list)
{
	static t_ast_nde	*cmd_list_lcl;

	if (cmd_list)
		cmd_list_lcl = cmd_list;
	else
	{
		free_sibling_and_child(cmd_list_lcl);
		cmd_list_lcl = NULL;	
	}
}
