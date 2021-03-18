/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/23 21:43:23 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/18 22:24:01 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		expander(t_btree *ast, t_list *env)
{
	t_token	*token;
	int		ret;

	ret = 1;
	if (!ast)
		return (1);
	token = ast->item;
	if (token->type == TK_WORD)
		ret = expand(token, env);
	if (ret != 1)
		return (ret);
	if (token->type == TK_WORD)
	{
		if ((ret = expander(ast->right, env)) != 1)
			return (ret);
	}
	return (1);
}

int		expand(t_token *token, t_list *env)
{
	int	ret;
	int	i;

	i = 0;
	ret = 1;
	while (token->data[i])
	{
		if (token->data[i] == '\'')
			ret = expand_single_quotes(token, &i);
		else if (token->data[i] == '\"')
			ret = expand_double_quotes(token, env, &i, ft_strlen(token->data));
		else if (token->data[i] == '$' && token->data[i + 1])
		{
			token->data = expand_dollar_sign(token->data, env, &i);
			if (!token->data)
				return (0);
		}
		if (ret != 1)
			return (ret);
		i++;
	}
	return (1);
}

char	*expand_dollar_sign(char *data, t_list *env, int *pos)
{
	char	*new_data;
	char	*var;
	int		length;

	if (ft_strncmp(data + *pos, "$?", 2) == 0)
	{
		var = ft_itoa(g_global.exit_code);
		length = 2;
	}
	else
		var = get_environment_var(env, data + *pos, &length);
	if (!(new_data = replace_env_var(data, var, *pos, length)))
		return (NULL);
	if (pos)
		*pos += length;
	free(data);
	return (new_data);
}

char	*replace_env_var(char *data, char *var, int i, int length)
{
	char	*new_data;

	if (var)
	{
		new_data = ft_strjoin(var, data + i + length);
		if (!new_data)
			return (NULL);
		free(var);
		data[i] = '\0';
		var = new_data;
		new_data = ft_strjoin(data, new_data);
		if (!new_data)
			return (NULL);
		free(var);
	}
	else
		new_data = ft_calloc(1, 1);
	return (new_data);
}

char	*expand_backslash(char c)
{
	char	*str;

	if (ft_strchr("$\\\"", c))
	{
		str = malloc(2);
		if (!str)
			return (NULL);
		str[0] = c;
		str[1] = '\0';
	}
	else
	{
		str = malloc(3);
		if (!str)
			return (NULL);
		str[0] = '\\';
		str[1] = c;
		str[2] = '\0';
	}
	return (str);
}
