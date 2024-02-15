/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dan <dan@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 19:30:35 by dan               #+#    #+#             */
/*   Updated: 2024/02/14 20:13:04 by dan              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	display_command_tab(char **command_tab);

int	str_is_digit(char *str)
{
	int	i;
	
	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}


int	exec_exit(t_Data *data, char **command_tab)
{
	data->exit_status = 0;

	ft_printf("exit\n");
	if (command_tab[1])
	{
		// data->exit_status = ft_atoi(command_tab[1]);
		if (!str_is_digit(command_tab[1]))
		{
			ft_printf("minishell: exit: %s: numeric argument required\n", command_tab[1]);
			data->exit_status = 2;
		}
		else 
			data->exit_status = ft_atoi(command_tab[1]);
		if (str_is_digit(command_tab[1]) && command_tab[2])
		{
			ft_printf("minishell: exit: too many arguments\n");
			data->exit_status = 1;
		}
	}
	exit(data->exit_status);
}

/* 
bash: exit: sd: numeric argument required > 2

bash: exit: too many arguments > 1

 */