/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/23 21:43:23 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/01 20:08:04 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		expander(t_btree *ast, t_list *env)
{
	t_token	*token;

	if (!ast)
		return (1);
	token = ast->item;
	if (token->type == TK_SINGLE_QUOTE || token->type == TK_DOUBLE_QUOTE)
	{
		if (!expand_quotes(token, env))
			return (0);
	}
	else if (token->type >= 60 && token->type <= 62)
	{
		if (!expand_quotes(token, env))
			return (0);
	}
	else if (token->type == TK_ENV_VAR)
	{
		if (!expand_dollar_sign(token, env))
			return (0);
		token->type = TK_WORD;
	}
	if (token->type == TK_WORD)
		expander(ast->right, env);
	return (1);
}

int		expand_quotes(t_token *token, t_list *env)
{
	char	*tmp;

	tmp = token->data;
	if (tmp[0] == '\'' || tmp[0] == '\"')
	{
		if (tmp[0] == '\'')
			token->data = ft_strtrim(token->data, "\'");
		else if (tmp[0] == '\"')
		{
			token->data = ft_strtrim(token->data, "\"");
			if (!expand_dollar_sign(token, env))
				return (0);
			expand_backslash(token);
		}
		free(tmp);
		if (token->data == NULL)
			return (0);
	}
	else if (!expand_dollar_sign(token, env))
		return (0);
	if (!is_operator(token))
		token->type = TK_WORD;
	return (1);
}

int		expand_dollar_sign(t_token *token, t_list *env)
{
	char	*var;
	char	*new_data;
	int		length;
	int		i;

	i = -1;
	if (token->data == NULL)
		return (0);
	while (token->data[++i])
	{
		if (token->data[i] == '$')
		{
			var = get_environment_var(env, token->data + i, &length);
			if (var)
			{
				new_data = ft_strjoin(var, token->data + i + length);
				if (!new_data)
					return (0);
				token->data[i] = '\0';
				i += ft_strlen(var);
				var = new_data;
				new_data = ft_strjoin(token->data, new_data);
				free(var);
				if (!new_data)
					return (0);
				free(token->data);
				token->data = new_data;
			}
		}
		else if (token->data[i] == '\\' && token->data[i + 1] == '$')
			i++;
	}
	return (1);
}

void	expand_backslash(t_token *token)
{
	char	*str;
	int		i;

	i = 0;
	while (token->data[i])
	{
		if (token->data[i] == '\\')
		{
			if (ft_strchr("$\\\"", token->data[i + 1]))
				token->data[i] = token->data[i + 1];
			else if (token->data[i + 1] == 'n')
				token->data[i] = '\n';
			str = token->data + i + 2;
			ft_strlcpy(token->data + i + 1, str, ft_strlen(str - 1));
		}
		i++;
	}
}
