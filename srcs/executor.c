/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/23 21:24:30 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/16 01:18:00 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exec_ast(t_btree *ast, t_env *vars, int *errcode)
{
	if (!ast)
		return (0);
	if (ast->item == NULL)
		return (1);
	if (!exec_node(ast, ast, vars, errcode))
		return (0);
	return (1);
}

int	exec_node(t_btree *root, t_btree *node, t_env *vars, int *errcode)
{
	t_token	*token;
	int		ret;

	if (!node)
		return (1);
	if ((ret = expander(node, vars->env)) != 1)
		return (ret);
	token = node->item;
	token->code = errcode;
	if (token->type >= TK_INPUT && token->type <= TK_OUTPUT_APPEND)
		return (handle_redirect(root, node, vars));
	else if (token->type == TK_PIPE)
		return (handle_pipe(root, node, vars));
	else if (token->type == TK_WORD)
		return (handle_word(root, node, vars));
	else
	{
		if (!exec_node(root, node->left, vars, errcode))
			return (0);
		if (!exec_node(root, node->right, vars, errcode))
			return (0);
		return (1);
	}
}
