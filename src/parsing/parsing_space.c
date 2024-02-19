/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_space.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seblin <seblin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 15:59:03 by seblin            #+#    #+#             */
/*   Updated: 2024/02/09 13:19:52 by seblin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "parsing_utils.h"

t_ast_nde	*copy_node(t_ast_nde *node);
void	print_raw_rght(t_ast_nde *raw_rght);
t_ast_nde	*set_chevron(t_ast_nde *node);
t_ast_nde	*set_dollar(t_ast_nde *node);
static void	fill_child(t_ast_nde *sib, t_ast_nde *raw_lft, t_ast_nde *raw_rght, t_ast_nde *token)
{
	t_ast_nde	*raw_lft_child_sav;
	t_ast_nde	*raw_rght_child_sav;
	t_ast_nde	*raw_overlap;

	raw_lft_child_sav = NULL;
	raw_rght_child_sav = NULL;	
	while (sib)
	{		
		if (sib->token != RAW)
		{
			if 	(sib->end < token->start)
				add_sibling(copy_node(sib), &raw_lft->child, &raw_lft_child_sav);				
			else if (sib->start > token->end)
				add_sibling(copy_node(sib), &raw_rght->child, &raw_rght_child_sav);	
		}
		else
		{
			if 	(sib->end < token->start)
				add_sibling(copy_node(sib), &raw_lft->child, &raw_lft_child_sav);				
			else if (sib->start > token->end)
				add_sibling(copy_node(sib), &raw_rght->child, &raw_rght_child_sav );
			else if (sib->start <= token->start && sib->end >= token->end)
			{
				if (sib->start < token->start)
				{
					raw_overlap = copy_node(sib);
					raw_overlap->end = token->start - 1;
					add_sibling(raw_overlap, &raw_lft->child, &raw_lft_child_sav);					
				}
				if (sib->end > token->end)
				{					
					raw_overlap = copy_node(sib);
					raw_overlap->start = token->end + 1;
					add_sibling(raw_overlap, &raw_rght->child, &raw_rght_child_sav);
				}
			}			
		}		
		sib = sib->sibling;
	}
	if (raw_lft)
		raw_lft->child = raw_lft_child_sav;
	if (raw_rght)
		raw_rght->child = raw_rght_child_sav;	
}
static t_ast_nde	*create_space_node(t_ast_nde *sib)
{
	t_ast_nde	*token_nde;
	char		*actual;
	
	token_nde = NULL;
	while (sib)
	{
		actual = sib->start;	
		while (sib->token == RAW && actual <= sib->end)
		{
			if (*actual == ' ')
			{
				token_nde = create_node(SPCE);
				token_nde->start = actual;
				token_nde->end = actual;
				while (*++actual == ' ')								
					token_nde->end = actual;				
				return (token_nde);
			}		
			actual++;
		}
		sib = sib->sibling;
	}
	return (token_nde);
}
static t_ast_nde	*create_token_child(t_ast_nde *raw, t_ast_nde *token)
{		
	t_ast_nde	*raw_lft; 
	t_ast_nde	*raw_rght;
		
	raw_lft = create_node(RAW);
	raw_lft->start = raw->start;
	raw_lft->end = token->start - 1;
	if (raw->start != token->start)
		raw_lft->child = copy_node(raw_lft);	
	raw_rght = create_node(RAW);
	raw_rght->start = token->end + 1;
	raw_rght->end = raw->end;
	if (raw->end != token->end)
		raw_rght->child = copy_node(raw_rght);
	raw_lft->sibling = raw_rght;
	return (raw_lft);
}

int	set_space(t_ast_nde *node)
{
	t_ast_nde *sib;
	t_ast_nde *sib_cont;
	t_ast_nde *token;
	t_ast_nde *raw_lft;
	t_ast_nde *raw_rght;
	
	sib_cont = node->child;
	sib = sib_cont->child;	
	token = create_space_node(sib);
	sib_cont->sibling = token;
	if (token)
	{
		raw_lft = create_token_child(sib_cont, token);
		raw_rght = raw_lft->sibling;	
		token->child = raw_lft;
		fill_child(sib, raw_lft->child, raw_rght->child, token);
		if (raw_lft->child)
			set_dollar(raw_lft);
		if (raw_rght->child)		
			set_space(raw_rght);
		return (1);
	}
	set_dollar(node);		
	return (0);
}