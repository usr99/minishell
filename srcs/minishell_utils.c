/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 18:11:57 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/18 22:43:50 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		execute(char *cmd, t_env *vars)
{
	t_btree	*ast;
	int		ret;

	ast = read_cmd(cmd);
	ret = exec_ast(ast, vars, &g_global.exit_code);
	btree_clear(&ast, &free_token);
	if (!ret)
		return (-1);
	return (0);
}

void	handle_signal_code(int signal)
{
	if (signal == SIGINT)
		g_global.exit_code = 130;
}
