/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree_clear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 17:34:03 by mamartin          #+#    #+#             */
/*   Updated: 2021/01/27 15:41:27 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

static void	clear(t_btree *root, void (*del)(void *))
{
	if (!root)
		return ;
	clear(root->left, del);
	clear(root->right, del);
	if (del)
		(*del)(root->item);
}

void		btree_clear(t_btree **root, void (*del)(void *))
{
	if (*root == NULL)
		return ;
	clear(*root, del);
	free(*root);
	*root = NULL;
}
