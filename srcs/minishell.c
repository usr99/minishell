/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 14:48:12 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/18 22:36:55 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		main(int ac, char **av, char **env)
{
	t_env	vars;
	int		ret;

	(void)ac;
	(void)av;
	vars.env = dup_env(env);
	vars.export = dup_env(env);
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	print_prompt();
	g_global.exit_code = 0;
	ret = shell_loop(&vars);
	ft_lstclear(&vars.env, &free);
	ft_lstclear(&vars.export, &free);
	ft_putendl_fd("exit", STDOUT_FILENO);
	return (ret);
}

int		shell_loop(t_env *vars)
{
	int		ret;

	g_global.cmd[0] = '\0';
	while ((ret = read(STDIN_FILENO, g_global.cmd, CMD_SIZE)) >= 0)
	{
		if (ret == 0 && g_global.cmd[0] == '\0')
			break ;
		if (g_global.cmd[ret - 1] == '\n')
		{
			g_global.cmd[ret - 1] = '\0';
			if (check_syntax(g_global.cmd) != -1)
			{
				if ((ret = execute(g_global.cmd, vars)) == -1)
					return (EXIT_FAILURE);
			}
			else
				print_error("syntax error", "minishell", NULL);
			g_global.cmd[0] = '\0';
			print_prompt();
		}
	}
	return (EXIT_SUCCESS);
}

void	handle_signal(int signal)
{
	if (signal == SIGINT)
	{
		g_global.exit_code = 130;
		g_global.cmd[0] = '\0';
		ft_putchar_fd('\n', STDOUT_FILENO);
		print_prompt();
	}
}

void	print_prompt(void)
{
	char	cwd[1024];

	ft_putstr_fd("\033[01;34m""\n", STDERR_FILENO);
	getcwd(cwd, sizeof(cwd));
	ft_putstr_fd(cwd, STDERR_FILENO);
	ft_putstr_fd("\n$> ", STDERR_FILENO);
	ft_putstr_fd("\033[00m", STDERR_FILENO);
}

void	print_error(char *message, char *name, char *cmd)
{
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(message, STDERR_FILENO);
	if (cmd)
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
	}
	ft_putchar_fd('\n', STDERR_FILENO);
	g_global.exit_code = 1;
}
