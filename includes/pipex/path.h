/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dan <dan@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 14:15:20 by seblin            #+#    #+#             */
/*   Updated: 2024/03/08 19:30:57 by dan              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATH_H
# define PATH_H

# include <stdlib.h>
# include "libft.h"
# include "ft_printf.h"
# include "minishell_struct.h"
# include "parsing_utils.h"

void	display_error(char *str);
void	display_error_detail(char *str, char *details, char *str2);

#endif