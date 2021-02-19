/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 14:52:40 by mamartin          #+#    #+#             */
/*   Updated: 2021/02/12 18:20:33 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin_unset(char **argv, t_list **env)
{
	t_list	*lst;
	t_list	*tmp;
	int		size_arg;
	int		size_var;
	int		i;

	if (how_many_arguments(argv) == 1)
	{
		ft_putstr_fd("unset: not enough arguments\n", STDERR_FILENO);
		return (1);
	}
	i = 1;
	while (argv[i])
	{
		lst = *env;
		tmp = NULL;
		size_arg = ft_strlen(argv[i]);
		while (lst)
		{
			size_var = ft_strchr(lst->content, '=') - (char *)lst->content;
			if (size_var < size_arg)
				size_var = size_arg;
			if (!ft_strncmp(argv[i], (char *)lst->content, size_var))
			{
				if (tmp)
					tmp->next = lst->next;
				else
					*env = (*env)->next;
				ft_lstdelone(lst, NULL);
				break ;
			}
			tmp = lst;
			lst = lst->next;
		}
		i++;
	}
	return (1);
}

int	builtin_env(char **argv, t_list **env)
{
	t_list	*lst;
	int		i;

	i = 0;
	if (how_many_arguments(argv) > 1)
	{
		ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	lst = *env;
	while (lst)
	{
		printf("%s\n", (char *)lst->content);
		lst = lst->next;
		i++;
	}
	return (1);
}

int	builtin_exit(char **argv, t_list **env)
{
	(void)env;
	if (how_many_arguments(argv) > 1)
	{
		ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	how_many_arguments(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		i++;
	return (i);
}
