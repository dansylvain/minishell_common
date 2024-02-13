/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dan <dan@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 18:46:30 by dan               #+#    #+#             */
/*   Updated: 2024/02/13 13:07:07 by dan              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>
void	exec_env(char **envp, char **command_tab)
{
	int	i;
	write(2, "IN EXC env!\n", 13);
	if (command_tab[1] != NULL)
	{
		ft_printf("env: ‘%s’: No such file or directory\n", command_tab[1]);
		return ;
	}
	else
	{
		i = 0;
		while (envp[i])
			ft_printf("%s\n", envp[i++]);
	}
	//exit(0);
}
