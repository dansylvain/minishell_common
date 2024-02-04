/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seblin <seblin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 10:45:59 by svidot            #+#    #+#             */
/*   Updated: 2024/02/04 17:46:16 by seblin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "libft.h"

#include "pipex_setup.h"

static void	close_fd(int fd_file[])
{
	if (fd_file[0] >= 0)
		close(fd_file[0]);
	if (fd_file[1] >= 0)
		close(fd_file[1]);
}

static char	*create_strerror(char *error_str, char *filepath)
{
	char	*tmp;
	char	*tmp2;
	char	*tmp3;
	char	*tmp4;

	tmp = ft_strjoin(strerror(errno), ": ");
	tmp2 = ft_strjoin(tmp, filepath);
	tmp3 = ft_strjoin(tmp2, "\n");
	tmp4 = NULL;
	if (*error_str)
		tmp4 = error_str;
	error_str = ft_strjoin(error_str, tmp3);
	free(tmp);
	free(tmp2);
	free(tmp3);
	if (tmp4)
		free(tmp4);
	return (error_str);
}

void	get_fdio(t_redir redir)
{
	char	*error_str;

	error_str = "";
	if (!flag)
	{
		fd_file[0] = open(filepaths[0], O_RDONLY);
		if (fd_file[0] < 0)
			error_str = create_strerror(error_str, filepaths[0]);
		fd_file[1] = open(filepaths[1], O_WRONLY | O_CREAT | O_TRUNC, 400);
		if (fd_file[1] < 0)
			error_str = create_strerror(error_str, filepaths[1]);
	}
	else
	{
		fd_file[0] = -1;
		fd_file[1] = open(filepaths[1], O_WRONLY | O_CREAT | O_APPEND, 400);
		if (fd_file[1] < 0)
			error_str = create_strerror(error_str, filepaths[1]);
	}
	if (*error_str)
		return (ft_putstr_fd(error_str, STDERR_FILENO), free(error_str),
			close_fd(fd_file), exit(EXIT_FAILURE));
}
