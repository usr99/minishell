/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/20 17:45:35 by mamartin          #+#    #+#             */
/*   Updated: 2021/01/22 17:07:28 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

typedef enum	e_tk_type
{
	TK_WORD,
	TK_SINGLE_QUOTE = 39,
	TK_DOUBLE_QUOTE = 34,
	TK_INPUT = 60,
	TK_OUTPUT,
	TK_OUTPUT_APPEND,
	TK_PIPE = 124,
	TK_ENV_VAR = 36,
	TK_SEMICOLON = 59,
}				t_tk_type;

typedef struct	s_token
{
	t_tk_type	type;
	char		*data;
}				t_token;

#endif
