/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 15:28:24 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/07 18:52:49 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_list	*dup_env(char **env)
{
	t_list	*begin;
	t_list	*new;
	char	*str;
	int		i;

	i = 0;
	begin = NULL;
	while (env[i])
	{
		str = ft_strdup(env[i]);
		if (!str)
		{
			ft_lstclear(&begin, &free);
			exit(EXIT_FAILURE);
		}
		new = ft_lstnew(str);
		if (!new)
		{
			ft_lstclear(&begin, &free);
			exit(EXIT_FAILURE);
		}
		ft_lstadd_back(&begin, new);
		i++;
	}
	return (begin);
}

char	*get_environment_var(t_list *env, char *name, int *length)
{
	char	*varname;
	int		i;
	int		j;

	if (*name == '$')
	{
		i = 1;
		while (!ft_strchr("\'\"$|;<> \\", name[i]) && name[i])
			i++;
		while (env)
		{
			j = 0;
			varname = (char *)env->content;
			while (varname[j] != '=' && varname[j])
				j++;
			if (!ft_strncmp(env->content, name + 1, j))
			{
				*length = i;
				return (ft_strdup(env->content + i));
			}
			env = env->next;
		}
	}
	return (NULL);
}
