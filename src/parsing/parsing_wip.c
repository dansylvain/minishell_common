/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_wip.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dan <dan@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 16:18:05 by seblin            #+#    #+#             */
/*   Updated: 2024/01/31 17:08:36 by dan              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "parsing_utils.h"
#include "libft.h"

#include "stdio.h"

static int	is_other(char *str)
{
	return (!(*(str - 1) != '\\' && (*str == '\'' || *str == '\"')));
}

static int	is_qute(char *str, char qute)
{
	return (*(str - 1) != '\\' && *str == qute);
}

static void	set_other_nde(t_ast_nde *other, char **str)
{
	other->start = *str;
	other->end = *str;
	
	while (*++(*str))
	{
		if (!is_other(*str))			
			break ;
		other->end = *str;			
	}	
}

static int set_qute_nde(t_ast_nde *qute_nde, char qute,
	t_ast_nde *sibling_sav, char **str)
{
	qute_nde->start = *str;
	while (*++(*str))
	{
		if (is_qute(*str, qute))
		{		
			qute_nde->end = *str;
			(*str)++;
			break ;
		}		
	}
	if (!qute_nde->end)
	{
		ft_putstr_fd("quote error\n", 2);
		free_sib(sibling_sav);
		free_sib(qute_nde);
		return (1);
	}
	return (0);
}
static void	link_qute_node(t_tok tok, char qute, t_ast_nde **qute_sibling,
	t_ast_nde **qute_sibling_sav, char **str)
{
	t_ast_nde	*new_nde;

	new_nde = create_node(tok);
	set_qute_nde(new_nde, qute, *qute_sibling_sav, str); 	
	add_sibling(new_nde, qute_sibling, qute_sibling_sav);
}

static t_ast_nde	*set_quote_sib(char *str)
{
	t_ast_nde	*qute_sibling_sav;
	t_ast_nde	*qute_sibling;
	t_ast_nde	*other_nde;

	qute_sibling_sav = NULL;
	while (*str)
	{
		if (is_qute(str, '\''))		
			link_qute_node(SQUTE, '\'', &qute_sibling, &qute_sibling_sav, &str);		
		else if (is_qute(str, '\"'))		
			link_qute_node(DQUTE,'\"', &qute_sibling, &qute_sibling_sav, &str);		
		else
		{
			other_nde = create_node(NONE); 
			set_other_nde(other_nde, &str);
			add_sibling(other_nde, &qute_sibling, &qute_sibling_sav);
		}	
	}
	return (qute_sibling_sav);
}

void	print_ast_sib(t_ast_nde *sib)
{
	int	i;	
	int	color;
	
	while (sib)
	{
		if (sib->token == NONE)
			color = 32;
		else if (sib->token == SQUTE)
			color = 31;
		else if (sib->token == DQUTE)
			color = 34;
		i = 0;
		while (sib->start + i <= sib->end)
			printf("\033[%dm%c\033[0m", color, sib->start[i++]);		
		sib = sib->sibling;
	}
	printf("\n");
}

static char	**create_ast(char *str)
{
	char		**ast_res;
	t_ast_nde	*qute_sib;
	
	qute_sib = set_quote_sib(str);
	print_ast_sib(qute_sib);
	free_sib(qute_sib);
	return (ast_res);
}

char	*parse(char *str)
{
	if (!*str)
		return (NULL);
	create_ast(str);
	return ("ok");
}
// int	tmp_main(void)
// {
// 	char *str = " 'e\"s't \"'m\"oi' bie'";
// 	parse(str);
// 	return(0);
// }