/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 16:14:33 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/06 16:14:55 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_pipe(t_btree *node, t_list **env)
{
	pid_t	pid;
	int		tmp;
	int		p[2];

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
	{
		tmp = dup(STDOUT_FILENO);
		close(p[0]);
		dup2(p[1], STDOUT_FILENO);
		close(p[1]);
		if (!exec_node(node->left, env, ((t_token *)(node->item))->code))
			exit(EXIT_FAILURE);
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
		if (!exec_node(node->right, env, ((t_token *)(node->item))->code))
			return (0);
		dup2(tmp, STDIN_FILENO);
		*((t_token *)(node->item))->code = 0;
		return (1);
	}
}
