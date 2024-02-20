/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dan <dan@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 19:30:35 by dan               #+#    #+#             */
/*   Updated: 2024/02/20 11:06:53 by dan              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**========================================================================
 *                           exec_exit
 *========================================================================**/
int	exec_exit(t_Data *data, char **command_tab)
{
	data->exit_status = 0;
	ft_printf("exit\n");
	if (command_tab[1])
	{
		if (!str_is_digit(command_tab[1]))
		{
			display_error_detail("minishell: exit: ", command_tab[1],
				":numeric argument required\n");
			data->exit_status = 2;
		}
		else
			data->exit_status = ft_atoi(command_tab[1]);
		if (str_is_digit(command_tab[1]) && command_tab[2])
		{
			display_error("minishell: exit: too many arguments\n");
			data->exit_status = 1;
		}
	}
	exit(data->exit_status);
}

/**========================================================================
 *                           str_is_digit
 *========================================================================**/
int	str_is_digit(char *str)
{
	int	i;

	i = 0;
	if (!ft_isdigit(str[0]) && str[0] != '+' && str[0] != '-')
		return (0);
	i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}
