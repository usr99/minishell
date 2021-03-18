/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 02:08:17 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/18 22:24:28 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*create_str(char c)
{
	char	*str;

	str = malloc(2);
	if (!str)
		return (NULL);
	str[0] = c;
	str[1] = '\0';
	return (str);
}

char	*get_var(t_list *env, char *data, int *i)
{
	char	*var;
	int		length;

	if (ft_strncmp(data + *i, "$?", 2) == 0)
	{
		var = ft_itoa(g_global.exit_code);
		length = 2;
	}
	else
		var = get_environment_var(env, data + *i, &length);
	*i += length - 1;
	if (!var)
		var = ft_calloc(1, 1);
	return (var);
}

int		*get_quotes_index(int *index, char *data, char quote, int i)
{
	index[0] = i;
	index[1] = i + 1;
	while (data[index[1]])
	{
		if (data[index[1]] == '\\')
			index[1]++;
		else if (data[index[1]] == quote)
			break ;
		if (data[index[1]] != '\0')
			index[1]++;
	}
	return (index);
}

int		set_new_data(t_token *token, char *new)
{
	free(token->data);
	token->data = new;
	return (1);
}
