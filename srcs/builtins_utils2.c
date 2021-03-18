/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 13:07:41 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/18 21:43:49 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		check_existing_var(char *str, t_list *env)
{
	t_list	*lst;
	char	*var;
	int		str_len;
	int		var_len;

	lst = env;
	str_len = get_varname_length(str);
	while (lst)
	{
		var = lst->content;
		var_len = get_varname_length(var);
		if (var_len == str_len)
		{
			if (ft_strncmp(str, var, var_len) == 0)
			{
				free(var);
				lst->content = str;
				return (1);
			}
		}
		lst = lst->next;
	}
	return (0);
}

int		is_echo_option(char *str)
{
	int	i;

	if (str[0] != '-')
		return (-1);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (-1);
		i++;
	}
	return (0);
}

int		check_numeric_value(char **argv, int nb_args)
{
	int	i;

	i = 0;
	while (ft_isdigit(argv[1][i]) && argv[1][i])
		i++;
	if (argv[1][i] != '\0')
	{
		print_error("argument numérique nécessaire", "exit", NULL);
		return (2);
	}
	else
	{
		if (nb_args > 2)
		{
			print_error("too many arguments", "exit", NULL);
			return (-1);
		}
		else
			return (ft_atoi(argv[1]));
	}
}

void	print_export(t_list *export)
{
	t_list	*lst;
	char	*str;
	int		i;

	lst = export;
	while (lst)
	{
		i = 0;
		str = lst->content;
		while (str[i] != '=')
			i++;
		str[i] = '\0';
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(str, STDOUT_FILENO);
		if (ft_strncmp(str + i + 1, "\'\'", 2))
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(str + i + 1, STDOUT_FILENO);
			ft_putstr_fd("\"\n", STDOUT_FILENO);
		}
		else
			ft_putchar_fd('\n', STDOUT_FILENO);
		str[i] = '=';
		lst = lst->next;
	}
}
