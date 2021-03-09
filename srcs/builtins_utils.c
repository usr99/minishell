/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 16:30:10 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/09 22:42:19 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		is_builtin(t_btree *root, char **argv, t_env *vars)
{
	int	size;

	size = ft_strlen(argv[0]);
	if (ft_strncmp(argv[0], "echo", size) == 0)
		return (builtin_echo(argv));
	else if (ft_strncmp(argv[0], "cd", size) == 0)
		return (builtin_cd(argv));
	else if (ft_strncmp(argv[0], "pwd", size) == 0)
		return (builtin_pwd(argv));
	else if (ft_strncmp(argv[0], "export", size) == 0)
		return (builtin_export(argv, vars));
	else if (ft_strncmp(argv[0], "unset", size) == 0)
		return (builtin_unset(argv, vars));
	else if (ft_strncmp(argv[0], "env", size) == 0)
		return (builtin_env(argv, &vars->env));
	else if (ft_strncmp(argv[0], "exit", size) == 0)
		return (builtin_exit(root, argv, vars));
	else
		return (NOT_BUILTIN);
}

int		how_many_arguments(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		i++;
	return (i);
}

int		get_pwd(char **cwd)
{
	char	*tmp;
	int		size;

	size = PWD_STEP + 1;
	*cwd = NULL;
	while (*cwd == NULL)
	{
		*cwd = (char *)malloc(size);
		if (!(*cwd))
			return (0);
		tmp = getcwd(*cwd, size);
		if (!tmp && errno == ERANGE)
		{
			free(*cwd);
			*cwd = NULL;
		}
		size += PWD_STEP;
	}
	*cwd = tmp;
	return (1);
}

void	delete_var(char *argv, t_list **env)
{
	t_list	*lst;
	t_list	*tmp;
	int		size_arg;
	int		size_var;

	lst = *env;
	tmp = NULL;
	size_arg = ft_strlen(argv);
	while (lst)
	{
		size_var = ft_strchr(lst->content, '=') - (char *)lst->content;
		if (size_var < size_arg)
			size_var = size_arg;
		if (!ft_strncmp(argv, (char *)lst->content, size_var))
		{
			if (tmp)
				tmp->next = lst->next;
			else
				*env = (*env)->next;
			ft_lstdelone(lst, &free);
			break ;
		}
		tmp = lst;
		lst = lst->next;
	}
}

int		create_var(char *arg, t_list *env)
{
	char	*str;
	t_list	*new;

	if (ft_strchr(arg, '=') == NULL)
		str = ft_strjoin(arg, "=\'\'");
	else
		str = ft_strdup(arg);
	if (!str)
		return (0);
	new = ft_lstnew(str);
	if (!new)
		return (0);
	ft_lstlast(env)->next = new;
	return (1);
}
