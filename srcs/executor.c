/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/23 21:24:30 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/06 17:19:52 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exec_ast(t_btree *ast, t_list **env, int *errcode)
{
	if (!ast)
		return (0);
	if (ast->item == NULL)
		return (1);
	if (!exec_node(ast, env, errcode))
		return (0);
	return (1);
}

int	exec_node(t_btree *node, t_list **env, int *errcode)
{
	t_token	*token;
	int		ret;

	if (!node)
		return (1);
	if (!expander(node, *env, *errcode))
		return (0);
	token = node->item;
	token->code = errcode;
	if (token->type >= TK_INPUT && token->type <= TK_OUTPUT_APPEND)
		ret = handle_redirect(node, env);
	else if (token->type == TK_PIPE)
		ret = handle_pipe(node, env);
	else if (token->type == TK_WORD)
		ret = handle_word(node, env);
	else
	{
		if (!exec_node(node->left, env, errcode))
			return (0);
		if (!exec_node(node->right, env, errcode))
			return (0);
	}
	return (ret);
}
