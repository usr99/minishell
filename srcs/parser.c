/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 23:54:01 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/12 01:46:59 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_btree	*parser(t_list *lexer)
{
	t_btree	*ast;

	ast = NULL;
	if (!lexer)
	{
		ast = btree_create_node(NULL);
		return (ast);
	}
	if (!create_ast(&ast, lexer))
	{
		btree_clear(&ast, NULL);
		ast = NULL;
		ft_lstclear(&lexer, &free_token);
	}
	else
		ft_lstclear(&lexer, NULL);
	return (ast);
}

int		create_ast(t_btree **node, t_list *lexer)
{
	t_token	*tk;
	t_list	*lst;
	int		index;

	if (!lexer)
		return (1);
	tk = get_highest_token(lexer, &index);
	*node = btree_create_node(tk);
	if (*node == NULL)
		return (0);
	if (index != 0)
	{
		lst = ft_lstdup(lexer, index);
		if (!lst)
			return (0);
		if (!create_ast(&(*node)->left, lst))
			return (0);
		ft_lstclear(&lst, NULL);
	}
	while (index-- > -1 && lexer)
		lexer = lexer->next;
	if (!create_ast(&(*node)->right, lexer))
		return (0);
	return (1);
}

t_token	*get_highest_token(t_list *lexer, int *index)
{
	t_token	*curr;
	t_token	*tmp;
	int		i;

	i = 0;
	tmp = NULL;
	while (lexer)
	{
		curr = lexer->content;
		if (get_token_rank(tmp) < get_token_rank(curr))
		{
			*index = i;
			tmp = curr;
			if (curr->type == TK_SEMICOLON)
				return (tmp);
		}
		i++;
		lexer = lexer->next;
	}
	return (tmp);
}

int		get_token_rank(t_token *token)
{
	if (token == NULL)
		return (-1);
	else if (token->type == TK_SEMICOLON)
		return (3);
	else if (token->type == TK_PIPE)
		return (2);
	else if (token->type >= TK_INPUT && token->type <= TK_OUTPUT_APPEND)
		return (1);
	else
		return (0);
}

int		is_operator(t_token *token)
{
	if (token->type == TK_WORD)
		return (0);
	else
		return (1);
}
