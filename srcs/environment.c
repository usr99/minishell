/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 15:28:24 by mamartin          #+#    #+#             */
/*   Updated: 2021/01/27 15:28:44 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_list		*dup_env(char **env)
{
	t_list	*begin;
	t_list	*new;
	int		i;

	i = 0;
	begin = NULL;
	while (env[i])
	{
		new = ft_lstnew(env[i]);
		if (!new)
		{
			ft_lstclear(&begin, NULL);
			exit(EXIT_FAILURE);
		}
		ft_lstadd_back(&begin, new);
		i++;
	}
	return (begin);
}

char	*get_environment_var(t_list *env, char *name, int *length)
{
	int		i;

	if (*name == '$')
	{
		i = 1;
		while (!ft_strchr("\'\"$|;<> \\", name[i]) && name[i])
			i++;
		while (env)
		{
			if (!ft_strncmp(env->content, name + 1, i - 1))
			{
				*length = i;
				return (env->content + i);
			}
			env = env->next;
		}
	}
	return (NULL);
}