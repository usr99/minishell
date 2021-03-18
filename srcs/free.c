/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 03:51:28 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/18 18:23:18 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_token(void *ptr)
{
	t_token	*token;

	token = (t_token *)ptr;
	if (token)
	{
		if (token->data)
			free(token->data);
		free(token);
	}
}

void	destroy_all(t_btree *root, char **argv, t_env *vars)
{
	btree_clear(&root, &free_token);
	ft_lstclear(&vars->env, &free);
	ft_lstclear(&vars->export, &free);
	free(argv);
}
