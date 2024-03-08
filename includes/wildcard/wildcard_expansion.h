#ifndef WILDCARD_EXPANSION_H
#define WILDCARD_EXPANSION_H

#include <minishell_struct.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include "libft.h"
#include "parsing_utils.h"

int	get_str_size(char **matches);
void		launch_command_tab(t_Data *data, t_ast_nde *node,
				char *envp[], int flag);
char	**expand_wildcards(const char *pattern);
char	*copy_matches(char ***matches, char **ret_str,
			int index, char **pattern);
int		get_matching_elements(const char *pattern, DIR *dir,
			struct dirent *entry, char ***matches);

#endif