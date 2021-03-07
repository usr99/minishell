/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 18:42:17 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/07 18:12:44 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		handle_word(t_btree *root, t_btree *node, t_list **env)
{
	char	**argv;
	char	**envp;
	int		ret;

	argv = get_args(node);
	if (!argv)
		return (0);
	ret = is_builtin(root, argv, env);
	if (ret == NOT_BUILTIN)
	{
		envp = lst_to_array(*env);
		if (!envp)
			return (0);
		ret = exec_program(argv, envp);
		free(envp);
	}
	free(argv);
	*((t_token *)(node->item))->code = 0;
	if (ret == -1)
		*((t_token *)(node->item))->code = 1;
	return (ret);
}

char	**get_args(t_btree *node)
{
	t_list	*args;
	t_list	*new;
	t_token	*token;
	char	**array;

	args = NULL;
	while (node)
	{
		token = node->item;
		new = ft_lstnew(token->data);
		if (!new)
		{
			ft_lstclear(&args, NULL);
			return (NULL);
		}
		ft_lstadd_back(&args, new);
		node = node->right;
	}
	array = lst_to_array(args);
	ft_lstclear(&args, NULL);
	return (array);
}

char	**lst_to_array(t_list *lst)
{
	char	**array;
	int		i;

	i = 0;
	array = (char **)ft_calloc(ft_lstsize(lst) + 1, sizeof(char *));
	if (!array)
		return (NULL);
	while (lst)
	{
		array[i] = (char *)lst->content;
		lst = lst->next;
		i++;
	}
	return (array);
}

char	**get_path(char **envp)
{
	char	**path;
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp("PATH", envp[i], 4) == 0)
			break ;
		i++;
	}
	if (envp[i] == NULL)
		return (NULL);
	path = ft_split(envp[i] + 5, ':');
	return (path);
}
