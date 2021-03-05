/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/20 17:40:28 by mamartin          #+#    #+#             */
/*   Updated: 2021/02/21 17:07:14 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_btree		*read_cmd(char *cmd_line)
{
	t_list	*lst_tokens;
	t_list	*token;
	t_btree	*ast;
	int		i;

	i = 0;
	lst_tokens = NULL;
	cmd_line = ft_strtrim(cmd_line, " ");
	if (!cmd_line)
		return (NULL);
	while (cmd_line[i])
	{
		token = get_token(cmd_line, &i);
		if (!token)
		{
			ft_lstclear(&lst_tokens, &free_token);
			return (NULL);
		}
		ft_lstadd_back(&lst_tokens, token);
	}
	ast = parser(lst_tokens);
	free(cmd_line);
	return (ast);
}

t_list		*get_token(char *cmd, int *pos)
{
	t_tk_type	type;
	char		*data;

	data = NULL;
	while (cmd[*pos] == ' ' && cmd[*pos])
		(*pos)++;
	type = get_token_type(cmd + *pos);
	if (type == TK_SINGLE_QUOTE || type == TK_DOUBLE_QUOTE)
		data = get_token_data(cmd + *pos, type);
	else if (type == TK_WORD || type == TK_ENV_VAR)
		data = get_token_data(cmd + *pos, 0);
	else if (type >= TK_INPUT && type <= TK_OUTPUT_APPEND)
	{
		(*pos)++;
		if (type == TK_OUTPUT_APPEND)
			(*pos)++;
		while (cmd[*pos] == ' ' && cmd[*pos])
			(*pos)++;
		data = get_token_data(cmd + *pos, 0);
	}
	if (!data)
		*pos += 1;
	else
		*pos += ft_strlen(data);
	return (create_token_link(type, data));
}

t_tk_type	get_token_type(char *cmd)
{
	if (ft_strchr("\'\"$|;<", *cmd))
		return (*cmd);
	else if (*cmd == '>')
	{
		if (*(cmd + 1) == '>')
			return (TK_OUTPUT_APPEND);
		else
			return (TK_OUTPUT);
	}
	else
		return (TK_WORD);
}

char		*get_token_data(char *cmd, char type)
{
	char	*data;
	int		i;

	i = 1;
	if (type != 0)
	{
		while ((cmd[i] != type || cmd[i - 1] == '\\') && cmd[i])
			i++;
		if (cmd[i] == type)
			i++;
	}
	else
	{
		while (!ft_strchr("\'\"$|;<> ", cmd[i]) && cmd[i])
			i++;
	}
	data = ft_substr(cmd, 0, i);
	return (data);
}

t_list		*create_token_link(t_tk_type type, char *data)
{
	t_token	*token;
	t_list	*new;

	if ((type == 34 || type == 36 || type == 39 || !type ||
		(type <= 60 && type >= 62)) && !data)
		return (NULL);
	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
	{
		if (data)
			free(data);
		return (NULL);
	}
	token->type = type;
	token->data = data;
	new = ft_lstnew(token);
	if (!new)
	{
		if (data)
			free(data);
		free(token);
		return (NULL);
	}
	return (new);
}
