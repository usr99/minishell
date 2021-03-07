/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 16:14:33 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/07 20:54:53 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_pipe(t_btree *root, t_btree *node, t_list **env)
{
	t_ast	ast;
	pid_t	pid;
	int		tmp;
	int		p[2];

	ast.root = root;
	ast.node = node;
	tmp = pipe(p);
	pid = fork();
	if (tmp == -1 || pid < 0)
	{
		*((t_token *)(node->item))->code = 1;
		if (tmp == -1)
			print_error("pipe error", "minishell");
		else
			print_error("fork error", "minishell");
		return (-1);
	}
	else if (pid == 0)
		return (pipe_child(ast, env, p));
	else
		return (pipe_parent(ast, env, p, pid));
}

int	pipe_child(t_ast ast, t_list **env, int pipe[2])
{
	t_token	*token;
	int		tmp_fd;

	token = ast.node->item;
	if ((tmp_fd = my_dup(STDOUT_FILENO, -1, token)) == -1)
		exit(EXIT_ERR);
	close(pipe[0]);
	if (my_dup(pipe[1], STDOUT_FILENO, token) == -1)
		exit(EXIT_ERR);
	close(pipe[1]);
	if (!exec_node(ast.root, ast.node->left, env, token->code))
		exit(EXIT_FAILURE);
	if (my_dup(tmp_fd, STDOUT_FILENO, token) == -1)
		exit(EXIT_ERR);
	exit(EXIT_SUCCESS);
}

int	pipe_parent(t_ast ast, t_list **env, int pipe[2], int pid)
{
	t_token	*token;
	int		status;
	int		tmp_fd;

	waitpid(pid, &status, 0);
	kill(pid, SIGTERM);
	token = ast.node->item;
	if (status == EXIT_FAILURE)
		return (0);
	else if (status == EXIT_ERR)
		return (-1);
	if ((tmp_fd = my_dup(STDIN_FILENO, -1, token)) == -1)
		return (-1);
	close(pipe[1]);
	if (my_dup(pipe[0], STDIN_FILENO, token) == -1)
		return (-1);
	close(pipe[0]);
	if (!exec_node(ast.root, ast.node->right, env, token->code))
		return (0);
	if (my_dup(tmp_fd, STDIN_FILENO, token) == -1)
		return (-1);
	*token->code = 0;
	return (1);
}
