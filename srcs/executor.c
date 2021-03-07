/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/23 21:24:30 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/07 18:12:13 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exec_ast(t_btree *ast, t_list **env, int *errcode)
{
	if (!ast)
		return (0);
	if (ast->item == NULL)
		return (1);
	if (!exec_node(ast, ast, env, errcode))
		return (0);
	return (1);
}

int	exec_node(t_btree *root, t_btree *node, t_list **env, int *errcode)
{
	t_token	*token;

	if (!node)
		return (1);
	if (!expander(node, *env, *errcode))
		return (0);
	token = node->item;
	token->code = errcode;
	if (token->type >= TK_INPUT && token->type <= TK_OUTPUT_APPEND)
		return (handle_redirect(root, node, env));
	else if (token->type == TK_PIPE)
		return (handle_pipe(root, node, env));
	else if (token->type == TK_WORD)
		return (handle_word(root, node, env));
	else
	{
		if (!exec_node(root, node->left, env, errcode))
			return (0);
		if (!exec_node(root, node->right, env, errcode))
			return (0);
		return (1);
	}
}
