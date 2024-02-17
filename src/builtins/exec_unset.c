/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dan <dan@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 17:09:58 by dan               #+#    #+#             */
/*   Updated: 2024/02/17 14:02:01 by dan              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>
void	unset_var(char **envp, int index)
{
	int i;

	if (!ft_strncmp(envp[index], "_=", 2))
		return ;
	i = index;
	while (envp[i])
	{
		envp[i] = envp[i + 1];
		i++;
	}
	envp[i] = NULL;
}

void	exec_unset(t_Data *data, char **command_tab)
{
	int	i;
	int	j;
	int len;
	char *var;
	
	i = 1;
	while (command_tab[i])
	{
		len = ft_strlen(command_tab[i]);
		j = 0;
		while (data->envp_tab[j])
		{
			var = data->envp_tab[j];
			if (!ft_strncmp(var, command_tab[i], len) && (var[len] == '=' || !var[len]))
				unset_var(data->envp_tab, j);
			j++;
		}
		i++;
	}
}
