/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/23 21:24:30 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/01 20:07:37 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exec_ast(t_btree *ast, t_list **env)
{
	if (!ast)
		return (0);
	if (ast->item == NULL)
		return (1);
	if (!exec_node(ast, env))
		return (0);
	return (1);
}

int	exec_node(t_btree *node, t_list **env)
{
	t_token	*token;
	int		ret;

	if (!node)
		return (1);
	token = node->item;
	if (!expander(node, *env))
		return (0);
	if (token->type >= TK_INPUT && token->type <= TK_OUTPUT_APPEND)
	{
		ret = handle_redirect(token->type, node, env);
		if (!exec_node(node->right, env))
			return (0);
	}
	else if (token->type == TK_PIPE)
		ret = handle_pipe(node, env);
	else if (token->type == TK_WORD)
		ret = handle_word(node, env);
	else
	{
		if (!exec_node(node->left, env))
			return (0);
		if (!exec_node(node->right, env))
			return (0);
	}
	return (ret);
}

int	handle_redirect(t_tk_type type, t_btree *node, t_list **env)
{
	int	std_fd;
	int	std_save;
	int	fd;

	std_fd = STDOUT_FILENO;
	fd = open_file(type, ((t_token *)(node->item))->data);
	if (fd == -1)
	{
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		return (-1);
	}
	if (type == TK_INPUT)
		std_fd = STDIN_FILENO;
	std_save = dup(std_fd);
	if (dup2(fd, std_fd) == -1)
	{
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		return (-1);
	}
	if (!exec_node(node->left, env))
		return (0);
	close(fd);
	dup2(std_save, std_fd);
	close(std_save);
	return (1);
}

int	open_file(t_tk_type type, char *filename)
{
	int	fd;
	int	mode;

	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	if (type == TK_INPUT)
		fd = open(filename, O_RDONLY);
	else if (type == TK_OUTPUT)
		fd = open(filename, O_CREAT | O_WRONLY, mode);
	else
		fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, mode);
	return (fd);
}

int	handle_pipe(t_btree *node, t_list **env)
{
	pid_t	pid;
	int		tmp;
	int		p[2];

	tmp = pipe(p);
	pid = fork();
	if (tmp == -1 || pid < 0)
	{
		ft_putstr_fd("minishell: pipe error\n", STDERR_FILENO);
		return (-1);
	}
	else if (pid == 0)
	{
		tmp = dup(STDOUT_FILENO);
		close(p[0]);
		dup2(p[1], STDOUT_FILENO);
		close(p[1]);
		exec_node(node->left, env);
		dup2(tmp, STDOUT_FILENO);
		exit(EXIT_SUCCESS);
	}
	else
	{
		waitpid(pid, NULL, 0);
		kill(pid, SIGTERM);
		tmp = dup(STDIN_FILENO);
		close(p[1]);
		dup2(p[0], STDIN_FILENO);
		close(p[0]);
		exec_node(node->right, env);
		dup2(tmp, STDIN_FILENO);
		return (1);
	}
}
