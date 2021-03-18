/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 14:52:40 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/18 19:18:14 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin_unset(char **argv, t_env *vars)
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
		delete_var(argv[i], &vars->env);
		delete_var(argv[i], &vars->export);
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
		ft_putendl_fd((char *)lst->content, STDOUT_FILENO);
		lst = lst->next;
		i++;
	}
	return (1);
}

int	builtin_exit(t_btree *root, char **argv, t_env *vars)
{
	int	nb_args;
	int	exit_code;

	ft_putendl_fd("exit", STDOUT_FILENO);
	nb_args = how_many_arguments(argv);
	if (nb_args == 1)
		exit_code = EXIT_SUCCESS;
	else if (nb_args > 1)
		exit_code = check_numeric_value(argv, nb_args);
	if (exit_code == -1)
		return (-1);
	destroy_all(root, argv, vars);
	exit(exit_code);
}
