/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   program_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 16:17:23 by mamartin          #+#    #+#             */
/*   Updated: 2021/02/19 00:11:32 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		exec_program(char **argv, char **envp)
{
	char	**path;
	char	*binary;
	int		ret;
	
	if (!ft_strchr(argv[0], '/'))
	{
		path = get_path(envp);
		binary = get_program_path(path, argv[0]);
		if (!binary)
		{
			ft_putstr_fd("minishell: command not found\n", STDERR_FILENO);
			if (path)
				ft_free_map(path);
			return (-1);
		}
		ret = fork_process(binary, argv, envp);
		free(binary);
		ft_free_map(path);
	}
	else
		ret = fork_process(argv[0], argv, envp);
	return (ret);
}

char	**get_path(char **envp)
{
	char	**path;
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp("PATH", envp[i], 4) == 0)
			break ;
		i++;
	}
	if (envp[i] == NULL)
		return (NULL);
	path = ft_split(envp[i] + 5, ':');
	return (path);
}

char	*get_program_path(char **path, char *program)
{
	char		*binary;
	int			size;
	int			i;

	i = 0;
	if (!path)
		return (NULL);
	while (path[i])
	{
		size = ft_strlen(path[i]) + ft_strlen(program) + 2;
		binary = (char *)ft_calloc(size, 1);
		if (!binary)
			return (NULL);
		ft_strlcat(binary, path[i], size);
		ft_strlcat(binary, "/", size);
		ft_strlcat(binary, program, size);
		if (check_binary(binary))
			return (binary);
		free(binary);
		i++;
	}
	return (NULL);
}

int		check_binary(char *filename)
{
	struct stat	bin_stat;

	if (stat(filename, &bin_stat) == 0)
		return (1);
	else
		return (0);
}

int		fork_process(char *binary, char **argv, char **envp)
{
	pid_t	pid;

	if (!check_binary(binary))
	{
		ft_putstr_fd("minishell: command not found\n", STDERR_FILENO);
		return (-1);
	}
	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("minishell: fork error\n", STDERR_FILENO);
		return (-1);
	}
	else if (pid == 0)
	{
		if (execve(binary, argv, envp) == -1)
			ft_putstr_fd("minishell: execution failed\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, NULL, 0);
		kill(pid, SIGTERM);
		return (1);
	}
}
