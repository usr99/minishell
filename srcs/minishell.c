/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 14:48:12 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/06 00:11:28 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		main(int ac, char **av, char **env)
{
	t_list	*lst_env;
	int		ret;

	(void)ac;
	(void)av;
	lst_env = dup_env(env);
	signal(SIGINT, handle_signal);
	print_prompt();
	ret = shell_loop(lst_env);
	ft_lstclear(&lst_env, NULL);
	ft_putchar_fd('\n', 1);
	return (ret);
}

int		shell_loop(t_list *lst_env)
{
	t_btree	*ast;
	char	cmd_line[CMD_SIZE + 1];
	int		exit_code;
	int		ret;

	ast = NULL;
	exit_code = 0;
	while ((ret = read(STDIN_FILENO, cmd_line, CMD_SIZE)) > 0)
	{
		cmd_line[ret - 1] = '\0';
		ast = read_cmd(cmd_line);
		ret = exec_ast(ast, &lst_env, &exit_code);
		btree_clear(&ast, &free_token);
		if (!ret)
			return (EXIT_FAILURE);
		print_prompt();
	}
	return (EXIT_SUCCESS);
}

void	handle_signal(int signal)
{
	(void)signal;
	ft_putchar_fd('\n', STDOUT_FILENO);
	print_prompt();
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

void	print_error(char *message, char *name)
{
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(message, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
}
