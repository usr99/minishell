/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 23:54:01 by mamartin          #+#    #+#             */
/*   Updated: 2021/01/23 02:09:40 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_btree	*parser(t_list *lexer)
{
	t_btree	*ast;

	ast = NULL;
	get_node(&ast, lexer);
	if (!ast)
		return (NULL);
	lexer = lexer->next;
	if (lexer)
	{
		if (!add_next_node(&ast, ast, lexer))
			ft_lstclear(&lexer, &free_token);
		else
			ft_lstclear(&lexer, NULL);
	}
	return (ast);
}

t_btree	*get_node(t_btree **root, t_list *lexer)
{
	t_btree	*node;

	node = btree_create_node(lexer->content);
	if (!node)
	{
		btree_clear(*root, NULL);
		return (NULL);
	}
	if (*root == NULL)
		*root = node;
	return (node);
}

int		add_next_node(t_btree **root, t_btree *prev, t_list *lexer)
{
	t_btree	*node;

	node = get_node(root, lexer);
	if (!node)
		return (0);
	if (is_operator(node->item))
	{
		node->left = *root;
		*root = node;
	}
	else
	{
		if (prev->left)
			prev->right = node;
		else
			prev->left = node;
	}
	lexer = lexer->next;
	if (lexer)
	{
		if (!add_next_node(root, node, lexer))
			return (0);
	}
	return (1);
}

int		is_operator(t_token *token)
{
	if (token->type == TK_WORD || token->type == TK_ENV_VAR)
		return (0);
	else if (token->type == TK_SINGLE_QUOTE || token->type == TK_DOUBLE_QUOTE)
		return (0);
	else
		return (1);
}
