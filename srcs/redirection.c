/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 16:13:28 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/09 21:42:49 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_redirect(t_btree *root, t_btree *node, t_env *vars)
{
	t_token	*token;
	int		std_fd;
	int		std_save;
	int		fd;

	token = node->item;
	fd = open_file(token, &std_fd);
	if (fd == -1)
		return (-1);
	if ((std_save = my_dup(std_fd, -1, token)) == -1)
		return (-1);
	if (my_dup(fd, std_fd, token) == -1)
		return (-1);
	if (!exec_node(root, node->left, vars, token->code))
		return (0);
	close(fd);
	if (my_dup(std_save, std_fd, token) == -1)
		return (-1);
	close(std_save);
	*token->code = 0;
	if (!exec_node(root, node->right, vars, token->code))
		return (0);
	return (1);
}

int	my_dup(int fd1, int fd2, t_token *token)
{
	int	dup_fd;

	if (fd2 == -1)
		dup_fd = dup(fd1);
	else
		dup_fd = dup2(fd1, fd2);
	if (dup_fd == -1)
	{
		*token->code = 1;
		print_error(strerror(errno), "minishell", NULL);
	}
	return (dup_fd);
}

int	open_file(t_token *token, int *std_fd)
{
	int	fd;
	int	mode;

	*std_fd = STDOUT_FILENO;
	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	if (token->type == TK_INPUT)
	{
		fd = open(token->data, O_RDONLY);
		*std_fd = STDIN_FILENO;
	}
	else if (token->type == TK_OUTPUT)
		fd = open(token->data, O_CREAT | O_WRONLY, mode);
	else
		fd = open(token->data, O_CREAT | O_WRONLY | O_APPEND, mode);
	if (fd == -1)
	{
		*token->code = 1;
		print_error(strerror(errno), "minishell", NULL);
	}
	return (fd);
}
