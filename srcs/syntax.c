/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 21:52:37 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/18 17:30:26 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_syntax(char *cmd)
{
	int		i;

	i = 0;
	while (cmd[i])
	{
		if ((cmd[i] == '\'' || cmd[i] == '\"'))
		{
			if (check_quotes(cmd, &i) == -1)
				return (-1);
		}
		else if (is_word(cmd[i]) == -1)
		{
			if (check_operator(cmd, &i) == -1)
				return (-1);
		}
		i++;
	}
	return (0);
}

int	check_quotes(char *cmd, int *i)
{
	char	quote;

	quote = cmd[*i];
	*i += 1;
	while (cmd[*i])
	{
		if (cmd[*i] == '\\')
			*i += 1;
		else if (cmd[*i] == quote)
			break ;
		if (cmd[*i] != '\0')
			(*i)++;
	}
	if (cmd[*i] == '\0')
		return (-1);
	return (0);
}

int	check_operator(char *cmd, int *i)
{
	int	is_semicolon;

	is_semicolon = 0;
	if (cmd[*i] == ';')
		is_semicolon = 1;
	*i += 1;
	if (cmd[*i] == '>')
		*i += 1;
	while (cmd[*i] == ' ' && cmd[*i])
		(*i)++;
	if (cmd[*i] == '>' || cmd[*i] == '<')
		return (0);
	if (is_word(cmd[*i]) == -1)
		return (-1);
	if (cmd[*i] == '\0' && !is_semicolon)
		return (-1);
	return (0);
}

int	is_word(char c)
{
	if (c == '<' || c == '>')
		return (-1);
	else if (c == '|' || c == ';')
		return (-1);
	return (1);
}
