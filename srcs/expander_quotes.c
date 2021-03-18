/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 01:44:13 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/15 23:34:27 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		expand_single_quotes(t_token *token, int *pos)
{
	char	*new;
	int		index[2];
	int		size;
	int		i;

	size = ft_strlen(token->data);
	get_quotes_index(index, token->data, '\'', *pos);
	i = 2;
	new = remove_squotes(token, index, i);
	if (!new)
		return (0);
	*pos = index[1] - (size - ft_strlen(new));
	return (set_new_data(token, new));
}

char	*remove_squotes(t_token *token, int *index, int i)
{
	char	*new;
	char	*tmp;

	new = ft_strdup(token->data + index[1] + 1);
	if (!new)
		return (NULL);
	while (i--)
	{
		token->data[index[i]] = '\0';
		tmp = new;
		if (i == 1)
			new = ft_strjoin(token->data + index[0] + 1, new);
		else
			new = ft_strjoin(token->data, new);
		free(tmp);
		if (!new)
			return (NULL);
	}
	return (new);
}

int		expand_double_quotes(t_token *token, t_list *env, int *pos, int size)
{
	char	*new;
	char	*next;
	char	*tmp;
	int		index[2];
	int		i;

	new = NULL;
	get_quotes_index(index, token->data, '\"', *pos);
	i = index[0] + 1;
	while (i < index[1])
	{
		if ((next = dquotes_expansion(token, env, &i)) == NULL)
			return (0);
		tmp = new;
		if ((new = ft_strjoin(tmp, next)) == NULL)
			return (0);
		free(tmp);
		free(next);
		i++;
	}
	if ((new = join_data(token, index, new)) == NULL)
		return (0);
	*pos = index[1] - (size - ft_strlen(new));
	return (set_new_data(token, new));
}

char	*dquotes_expansion(t_token *token, t_list *env, int *i)
{
	char	*next;

	if (token->data[*i] == '\\')
	{
		next = expand_backslash(token->data[*i + 1]);
		(*i)++;
	}
	else if (token->data[*i] == '$' && token->data[*i + 1] != '\"')
		next = get_var(env, token->data, i);
	else
		next = create_str(token->data[*i]);
	return (next);
}

char	*join_data(t_token *token, int *index, char *new)
{
	char	*tmp;

	tmp = new;
	new = ft_strjoin(new, token->data + index[1] + 1);
	if (!new)
		return (NULL);
	free(tmp);
	token->data[index[0]] = '\0';
	tmp = new;
	new = ft_strjoin(token->data, new);
	free(tmp);
	return (new);
}
