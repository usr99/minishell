/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 14:52:40 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/09 16:23:54 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin_unset(char **argv, t_list **env)
{
	int		i;

	if (how_many_arguments(argv) == 1)
	{
		print_error("not enough arguments", "unset", NULL);
		return (-1);
	}
	i = 1;
	while (argv[i])
	{
		delete_var(argv[i], env);
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
		print_error("too many arguments", "env", NULL);
		return (-1);
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

int	builtin_exit(t_btree *root, char **argv, t_list **env)
{
	int	nb_args;
	int	exit_code;

	nb_args = how_many_arguments(argv);
	if (nb_args > 2)
	{
		print_error("too many arguments", "exit", NULL);
		return (-1);
	}
	else if (nb_args == 1)
		exit_code = EXIT_SUCCESS;
	else
		exit_code = ft_atoi(argv[1]);
	btree_clear(&root, &free_token);
	ft_lstclear(env, &free);
	free(argv);
	exit(exit_code);
}
