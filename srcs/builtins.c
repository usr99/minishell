/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 18:44:56 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/09 22:37:14 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin_echo(char **argv)
{
	int	newline;
	int	i;

	i = 1;
	newline = 1;
	if (how_many_arguments(argv) > 1)
	{
		if (!ft_strncmp(argv[1], "-n", 2))
		{
			newline = 0;
			i++;
		}
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (argv[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (1);
}

int	builtin_cd(char **argv)
{
	int		i;

	i = how_many_arguments(argv);
	if (i > 2)
	{
		print_error("too many arguments", "cd", NULL);
		return (-1);
	}
	else if (i < 2)
	{
		print_error("not enough arguments", "cd", NULL);
		return (-1);
	}
	else
	{
		if (chdir(argv[1]) == -1)
		{
			print_error("can't open directory", "cd", NULL);
			return (-1);
		}
		return (1);
	}
}

int	builtin_pwd(char **argv)
{
	char	*cwd;

	if (how_many_arguments(argv) > 1)
	{
		print_error("too many arguments", "pwd", NULL);
		return (-1);
	}
	if (!get_pwd(&cwd))
		return (0);
	if (cwd)
		printf("%s\n", cwd);
	else
	{
		ft_putstr_fd("pwd: ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putchar_fd('\n', STDERR_FILENO);
	}
	free(cwd);
	return (1);
}

int	builtin_export(char **argv, t_env *vars)
{
	int		i;

	i = 1;
	if (how_many_arguments(argv) == 1)
	{
		sort_alpha_lst(&vars->export);
		builtin_env(argv, &vars->export);
	}
	else
	{
		while (argv[i])
		{
			if (!create_var(argv[i], vars->export))
				return (0);
			if (ft_strchr(argv[i], '=') != NULL)
			{
				if (!create_var(argv[i], vars->env))
					return (0);
			}
			i++;
		}
	}
	return (1);
}
