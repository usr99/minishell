/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 14:48:12 by mamartin          #+#    #+#             */
/*   Updated: 2021/02/18 22:48:01 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			main(int ac, char **av, char **env)
{
	t_list	*lst_env;
	t_btree	*ast;
	char	cmd_line[CMD_SIZE + 1];
	int		ret;

	(void)ac;
	(void)av;
	lst_env = dup_env(env);
	signal(SIGINT, handle_signal);
	print_prompt();
	ast = NULL;
	while ((ret = read(STDIN_FILENO, cmd_line, CMD_SIZE)) > 0)
	{
		cmd_line[ret - 1] = '\0';
		ast = read_cmd(cmd_line);
		if (!exec_ast(ast, &lst_env))
			break ;
		else
			btree_clear(&ast, &free_token);
		print_prompt();
	}
	btree_clear(&ast, &free_token);
	ft_lstclear(&lst_env, NULL);
	ft_putchar_fd('\n', 1);
	return (0);
}

void		handle_signal(int signal)
{
	(void)signal;
	ft_putchar_fd('\n', STDOUT_FILENO);
	print_prompt();
}

void		print_prompt(void)
{
	char	cwd[1024];

	ft_putstr_fd("\033[01;34m""\n", STDOUT_FILENO);
	getcwd(cwd, sizeof(cwd));
	ft_putstr_fd(cwd, STDOUT_FILENO);
	ft_putstr_fd("\n$> ", STDOUT_FILENO);
	ft_putstr_fd("\033[00m", STDOUT_FILENO);
}
