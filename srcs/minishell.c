/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 14:48:12 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/09 16:44:03 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		g_exit_code;

int		main(int ac, char **av, char **env)
{
	t_env	vars;
	int		ret;

	(void)ac;
	(void)av;

	vars.env = dup_env(env);
	vars.export = dup_env(env);
	
	signal(SIGINT, handle_signal);
	print_prompt();
	ret = shell_loop(vars);
	ft_lstclear(&lst_env, &free);
	ft_putchar_fd('\n', 1);
	return (ret);
}

int		shell_loop(t_env vars)
{
	t_btree	*ast;
	char	cmd_line[CMD_SIZE + 1];
	int		ret;

	ast = NULL;
	g_exit_code = 0;
	while ((ret = read(STDIN_FILENO, cmd_line, CMD_SIZE)) > 0)
	{
		cmd_line[ret - 1] = '\0';
		ast = read_cmd(cmd_line);
		ret = exec_ast(ast, &lst_env, &g_exit_code);
		btree_clear(&ast, &free_token);
		if (!ret)
			return (EXIT_FAILURE);
		print_prompt();
	}
	return (EXIT_SUCCESS);
}

void	handle_signal(int signal)
{
	if (signal == SIGINT)
	{
		g_exit_code = 130;
		ft_putchar_fd('\n', STDOUT_FILENO);
		print_prompt();
	}
}

void	print_prompt(void)
{
	char	cwd[1024];

	ft_putstr_fd("\033[01;34m""\n", STDOUT_FILENO);
	getcwd(cwd, sizeof(cwd));
	ft_putstr_fd(cwd, STDOUT_FILENO);
	ft_putstr_fd("\n$> ", STDOUT_FILENO);
	ft_putstr_fd("\033[00m", STDOUT_FILENO);
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
}
