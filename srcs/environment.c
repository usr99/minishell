/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 15:28:24 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/13 23:37:29 by mamartin         ###   ########.fr       */
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

char	*get_environment_var(t_list *env, char *name, int *len)
{
	char	*varname;
	int		i;
	int		j;

	if (*name == '$')
	{
		i = 1;
		while (!ft_strchr("\'\"$|;<> \\/", name[i]) && name[i])
			i++;
		while (env)
		{
			j = 0;
			varname = (char *)env->content;
			while (varname[j] != '=' && varname[j])
				j++;
			if (!ft_strncmp(varname, name + 1, j))
			{
				*len = i;
				return (ft_strdup(varname + i));
			}
			env = env->next;
		}
	}
	return (NULL);
}

void	useless_func(t_list **prev, t_list **curr)
{
	*prev = *curr;
	*curr = (*curr)->next;
}

void	sort_alpha_lst(t_list **lst)
{
	t_list	*curr;
	t_list	*prev;
	t_list	*tmp;

	curr = *lst;
	prev = NULL;
	while (curr->next)
	{
		tmp = curr->next;
		if (ft_strcmp(curr->content, tmp->content) > 0)
		{
			curr->next = tmp->next;
			tmp->next = curr;
			if (prev)
				prev->next = tmp;
			else
				*lst = tmp;
			curr = *lst;
			prev = NULL;
		}
		else
			useless_func(&prev, &curr);
	}
}

int		get_varname_length(char *var)
{
	int	length;

	length = 0;
	while (var[length] != '=' && var[length])
		length++;
	return (length);
}
