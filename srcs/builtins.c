/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 18:44:56 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/06 00:16:29 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	(*g_is_builtin(char *name))(char **, t_list **)
{
	int	size;

	size = ft_strlen(name);
	if (ft_strncmp(name, "echo", size) == 0)
		return (&builtin_echo);
	else if (ft_strncmp(name, "cd", size) == 0)
		return (&builtin_cd);
	else if (ft_strncmp(name, "pwd", size) == 0)
		return (&builtin_pwd);
	else if (ft_strncmp(name, "export", size) == 0)
		return (&builtin_export);
	else if (ft_strncmp(name, "unset", size) == 0)
		return (&builtin_unset);
	else if (ft_strncmp(name, "env", size) == 0)
		return (&builtin_env);
	else if (ft_strncmp(name, "exit", size) == 0)
		return (&builtin_exit);
	else
		return (NULL);
}

int	builtin_echo(char **argv, t_list **env)
{
	int	newline;
	int	i;

	(void)env;
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

int	builtin_cd(char **argv, t_list **env)
{
	t_list	*envp;
	int		ret;
	int		i;

	i = how_many_arguments(argv);
	envp = *env;
	if (i > 2)
	{
		print_error("too many arguments", "cd");
		return (-1);
	}
	else if (i == 1)
	{
		while (envp)
		{
			if (ft_strncmp("HOME", (char *)envp->content, 4) == 0)
				break ;
			envp = envp->next;
		}
		if (!envp)
		{
			print_error("no home directory", "cd");
			return (-1);
		}
		ret = chdir((char *)envp->content + 5);
	}
	else
		ret = chdir(argv[1]);
	if (ret == -1)
	{
		print_error("can't open directory", "cd");
		return (-1);
	}
	return (1);
}

int	builtin_pwd(char **argv, t_list **env)
{
	char	*cwd;
	char	*tmp;
	int		size;

	(void)env;
	size = 51;
	cwd = NULL;
	if (how_many_arguments(argv) > 1)
	{
		print_error("too many arguments", "pwd");
		return (-1);
	}
	while (cwd == NULL)
	{
		cwd = (char *)malloc(size);
		if (!cwd)
			return (0);
		tmp = getcwd(cwd, size);
		if (!tmp && errno == ERANGE)
		{
			free(cwd);
			cwd = NULL;
		}
		size += 50;
	}
	if (tmp)
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

int	builtin_export(char **argv, t_list **env)
{
	t_list	*new;
	char	*str;
	int		i;

	i = 1;
	if (how_many_arguments(argv) == 1)
		builtin_env(argv, env);
	else
	{
		while (argv[i])
		{
			if (ft_strchr(argv[i], '=') == NULL)
				str = ft_strjoin(argv[i], "=\'\'");
			else
				str = ft_strdup(argv[i]);
			if (!str)
				return (0);
			new = ft_lstnew(str);
			if (!new)
				return (0);
			ft_lstlast(*env)->next = new;
			i++;
		}
	}
	return (1);
}
