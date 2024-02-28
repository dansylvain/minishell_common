/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dan <dan@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 09:06:02 by svidot            #+#    #+#             */
/*   Updated: 2024/02/26 12:32:29 by dan              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "ft_printf.h"
#include "minishell.h"

void	display_error(char *str);

static char	*try_paths(char **argv, char *env_find)
{
	char	*cmd;
	char	**split_colon;
	char	**split_colon_sav;
	char	*tmp;

	cmd = NULL;
	split_colon = ft_split(env_find, ':');
	split_colon_sav = split_colon;
	while (split_colon && *split_colon)
	{
		tmp = ft_strjoin(*split_colon++, "/");
		cmd = ft_strjoin(tmp, *argv);
		free(tmp);
		if (!access(cmd, X_OK))
		{
			free(*argv);
			*argv = cmd;
			break ;
		}
		free(cmd);
		cmd = NULL;
	}
	ft_free_ptr_arr((void **) split_colon_sav);
	return (cmd);
}

char	*search_env_var(char *envp[], char *env_to_find)
{
	char	*env_find;

	env_find = NULL;
	while (*envp)
	{
		if (!ft_strncmp(*envp++, env_to_find, ft_strlen(env_to_find)))
		{
			env_find = *--envp;
			break ;
		}
	}
	if (!env_find)
		return (NULL);
	env_find += ft_strlen(env_to_find);
	return (env_find);
}

char	**search_path(char *argv[], char *envp[])
{
	char	**split_arg;
	char	*env_find;

	if (!**argv)
		return (NULL);
	env_find = search_env_var(envp, "PATH=");
	if (!env_find)
	{
		ft_putstr_fd("env PATH not found.\n", 2);
		exit(127);
	}
	if (!try_paths(argv, env_find))
	{
		display_error_detail(argv[0], ": command ", "not found\n");
		exit(127);
	}
	return (split_arg);
}

char	*search_var(const t_ast_nde *node, t_Data *data)
{
	char	*str;
	char	*var;

	str = ft_strndup(node->start, node->end - node->start + 1);
	if (*(str + 1) == '?')
		var = ft_itoa(data->exit_status);
	else
		var = search_env_var(data->envp_tab, ft_strjoin_up(str + 1, "=", 0, 0));
	if (str)
		free(str);
	return (var);
}