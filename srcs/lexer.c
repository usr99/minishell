/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/20 17:40:28 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/12 21:52:29 by mamartin         ###   ########.fr       */
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
	if (type == TK_WORD)
		data = get_token_data(cmd + *pos);
	else if (type >= TK_INPUT && type <= TK_OUTPUT_APPEND)
	{
		(*pos)++;
		if (type == TK_OUTPUT_APPEND)
			(*pos)++;
		while (cmd[*pos] == ' ' && cmd[*pos])
			(*pos)++;
		data = get_token_data(cmd + *pos);
	}
	if (!data)
		*pos += 1;
	else
		*pos += ft_strlen(data);
	return (create_token_link(type, data));
}

t_tk_type	get_token_type(char *cmd)
{
	if (ft_strchr("|;<", *cmd))
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

char		*get_token_data(char *cmd)
{
	char	*data;
	int		is_quote;
	int		i;

	i = 1;
	is_quote = 0;
	if (cmd[0] == '\'' || cmd[0] == '\"')
		is_quote = 1;
	while ((!ft_strchr("|;<> ", cmd[i]) || is_quote) && cmd[i])
	{
		if (cmd[i] == '\'' || cmd[i] == '\"')
		{
			if (is_quote == 0)
				is_quote = 1;
			else
				is_quote = 0;
		}
		i++;
	}
	data = ft_substr(cmd, 0, i);
	return (data);
}

t_list		*create_token_link(t_tk_type type, char *data)
{
	t_token	*token;
	t_list	*new;

	if ((!type || (type <= 60 && type >= 62)) && !data)
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
