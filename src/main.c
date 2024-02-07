/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dan <dan@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 14:04:56 by dan               #+#    #+#             */
/*   Updated: 2024/02/07 15:47:06 by dan              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "parsing_utils.h"
char		**parse_cmd(char **command, char **env);
void		tmp_main();
t_ast_nde	*parse(char *str);
int			pipex(char *argv[], char *envp[]);

/**========================================================================
 *                             COMMENTS POLICY
 *? signal info at the end of a function's comment block 
 *? signal info in the main comment bloc
 *? add your name at the beggining of a comment
 *---
 *TODO finish builtin export
 *TODO fix memory leaks export
 *TODO fix memory leaks unset
 *========================================================================**/

/**========================================================================
 *                           main.c
 *? rl_catch_signals is a global variable (from readline lib)
 *? it is used to ignore SIGQUILL (see handle_signals.c)
*========================================================================**/
int	main(int argc, char **argv, char *envp[])
{
	t_Data	*data;

	// tmp_main();
	// return 0;
	rl_catch_signals = 0;
	data = (t_Data *)malloc(sizeof(t_Data));
	if (data == NULL)
		return (display_error("Error\n"), 255);
	data->envp_tab = duplicate_envp(data, envp);
	if (!data->envp_tab)
		return (display_error("Error\n"), free_data(data), 255);
	if (argc != 1)
		return (free_data(data), display_error("Usage: ./minishell\n"), 255);
	handle_signals();
	if (prompt_loop(data, envp) == 0)
		return (free_data(data), 0);
	return (0);
}

/**========================================================================
 *                           
 * possibility to add the path before prompt with this function
 * (write a function "build_prompt")
 * prompt = getcwd(NULL, 0);
 *========================================================================**/
int	prompt_loop(t_Data *data, char *envp[])
{
	char	*cmd[2];
	char	*prompt;

	cmd[1] = NULL;
	cmd[0] = NULL;
	while (1)
	{
		cmd[0] = readline("minishell> ");
		if (cmd[0] && *cmd[0])
		{
			add_history(cmd[0]);
		}
		if (cmd[0] == NULL)
			return (ft_printf("exit\n"), 0);
		if (command_is_builtin(cmd, data, envp) == 0)
			return (free(cmd[0]), 0);
		if (cmd[0])
			free(cmd[0]);
	}
	return (1);
}

char	**create_command_tab(t_ast_nde *node)
{
	int tree_length;
	char **commands_tab;
	int i;

	t_ast_nde *current;
	current = node;
	tree_length = 0;
	while (current)
	{
		ft_printf("current->token: %i\n", current->token);
		ft_printf("%i\n", current->end - current->start + 1);		
		if (current->token != PIPE && current->end - current->start + 1)
			tree_length++;
		current = current->sibling;
	}
	ft_printf("tree_length: %i\n", tree_length);
	commands_tab = (char **)malloc(sizeof(char *) * tree_length + 1);
	if (commands_tab == 0)
		return (NULL);
	i = 0;
	while (node)
	{
		if (node->token != PIPE && node->end - node->start + 1)
		{
			commands_tab[i] = ft_strndup(node->start, node->end - node->start + 1);
			if (commands_tab[i] == NULL)
				return (NULL);
			i++;
		}
		node = node->sibling;
	}
	commands_tab[i] = NULL;
	return (commands_tab);
}

void	display_command_tab(char **command_tab)
{
	int i;

	i = 0;
	ft_printf("command_tab: \n");
	while (command_tab[i])
	{
		ft_printf("commands_tab[%i]: >%s<\n", i,  command_tab[i]);
		i++;
	}
}

/**========================================================================
 *                           command_is_builtin
 * exit builtin implemented without extern function
 * 0 is returned, and the data struct is freed in calling function
 *========================================================================**/
int	command_is_builtin(char	*cmd[], t_Data *data, char *envp[])
{
	char	**cmd_tab;

	//  parse(cmd[0]); exit(1);
	cmd_tab = create_command_tab(parse(cmd[0]));	
	display_command_tab(cmd_tab);
	pipex(cmd_tab, envp);
	return (1);
	
	if (!cmd_tab)
		return (1);
	if (!cmd_tab[0])
		return (free_command_tab(cmd_tab), 1);
	if (!ft_strncmp(&(cmd_tab[0][ft_strlen(cmd_tab[0]) - 4]), "echo", 5))
		exec_echo(cmd_tab);
	if (!ft_strncmp(&(cmd_tab[0][ft_strlen(cmd_tab[0]) - 5]), "unset", 6))
		exec_unset(data->envp_tab, cmd_tab);
	if (!ft_strncmp(&(cmd_tab[0][ft_strlen(cmd_tab[0]) - 6]), "export", 7))
		exec_export(cmd_tab, data);
	if (!ft_strncmp(&(cmd_tab[0][ft_strlen(cmd_tab[0]) - 3]), "env", 4))
		exec_env(data->envp_tab, cmd_tab);
	if (!ft_strncmp(&(cmd_tab[0][ft_strlen(cmd_tab[0]) - 3]), "pwd", 4))
		exec_pwd();
	if (!ft_strncmp(&(cmd_tab[0][ft_strlen(cmd_tab[0]) - 2]), "cd", 3))
		exec_cd(cmd_tab);
	if (!ft_strncmp(&(cmd_tab[0][ft_strlen(cmd_tab[0]) - 4]), "exit", 5))
		return (ft_printf("exit\n"), free_command_tab(cmd_tab), 0);
	free_command_tab(cmd_tab);
	return (1);
}

/**========================================================================
 *                           duplicate_envp
 * var ENVP_TAB_SIZE defined in minishell.h
 *========================================================================**/
char	**duplicate_envp(t_Data *data, char *envp[])
{
	char	**envp_tab;
	int		i;

	envp_tab = (char **)ft_calloc(ENVP_TAB_SIZE, sizeof(char *));
	if (envp_tab == NULL)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		envp_tab[i] = (char *)ft_calloc((ft_strlen(envp[i]) + 1), sizeof(char));
		if (envp_tab[i] == NULL)
			return (NULL);
		ft_strlcpy(envp_tab[i], envp[i], ft_strlen(envp[i]) + 1);
		i++;
	}
	envp_tab[i] = NULL;
	return (envp_tab);
}
