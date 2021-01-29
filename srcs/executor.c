/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/23 21:24:30 by mamartin          #+#    #+#             */
/*   Updated: 2021/01/29 15:53:28 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		exec_cmd(t_btree *ast, t_list *env)
{
	if (!ast)
		return (0);
	if (!expander(ast, env))
		return (0);
	if (!exec_node(ast))
		return (0);
	return (1);
}

int		exec_node(t_btree *node)
{
	t_token	*token;

	token = node->item;
	if (token->type >= TK_INPUT && token->type <= TK_OUTPUT_APPEND)
	{
		// redirection
		// aller à droite
		// doit être un TK_WORD
		// ouvre le fichier avec le bon mode
		// redirige le flux standard approprié vers le file descriptor

		// continue à gauche
	}
	else if (token->type == TK_PIPE)
	{
		// pipe

		// continue à gauche
		// continue à droite
	}
	else if (token->type == TK_WORD)
	{
		// get arguments
		// aller à gauche
		// doit être un TK_WORD
		// renvoie un char ** avec tous les arguments
		// exec (builtin ou executable)

		// return
	}
	else
	{
		// continue à gauche
		// continue à droite
	}
}