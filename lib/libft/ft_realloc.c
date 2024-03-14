/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dan <dan@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 11:22:17 by dan               #+#    #+#             */
/*   Updated: 2024/03/14 13:32:04 by dan              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void *ft_realloc(void *ptr, size_t size)
{
	size_t	old_size;
	size_t	copy_size;
	void	*new_ptr;
	
	if (ptr == NULL)
		return (ft_calloc(1, size));
	if (size == 0)
	{
		free(ptr);
		return NULL;
	}
	new_ptr = ft_calloc(1, size);
	if (new_ptr == NULL)
		return NULL;
	old_size = sizeof(ptr);
		if (size < old_size)
			copy_size = size;
		else
			copy_size = old_size;
		memcpy(new_ptr, ptr, copy_size);
	free(ptr);
	return new_ptr;
}
