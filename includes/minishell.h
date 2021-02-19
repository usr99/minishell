/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 14:33:29 by mamartin          #+#    #+#             */
/*   Updated: 2021/02/17 19:04:43 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "headers.h"
# include "defines.h"
# include "types.h"

/*
**	MINISHELL FUNCTIONS
*/

void			handle_signal(int signal);
void			print_prompt(void);

/*
**	ENVIRONMENT VARIABLES FUNCTIONS
*/

t_list			*dup_env(char **env);
char			*get_environment_var(t_list *env, char *name, int *length);

/*
**	LEXER FUNCTIONS
*/

t_btree			*read_cmd(char *cmd_line);
t_list			*get_token(char *cmd, int *pos);
t_tk_type		get_token_type(char *cmd);
char			*get_token_data(char *cmd, char type);
t_list			*create_token_link(t_tk_type type, char *data);

/*
**	PARSER FUNCTIONS
*/

t_btree			*parser(t_list *lexer);
void			create_ast(t_btree **root, t_list *lexer);
t_btree			*get_node(t_btree **root, t_list *lexer);
int				add_next_node(t_btree **root, t_btree *prev, t_list *lexer);
int				is_operator(t_token *token);

/*
**	EXPANDER FUNCTIONS
*/

int				expander(t_btree *ast, t_list *env);
int				expand_quotes(t_token *token, t_list *env);
int				expand_dollar_sign(t_token *token, t_list *env);
void			expand_backslash(t_token *token);

/*
**	EXECUTOR FUNCTIONS
*/

int				exec_ast(t_btree *ast, t_list **env);
int				exec_node(t_btree *node, t_list **env);
int				handle_redirect(t_tk_type type, t_btree *node, t_list **env);
int				handle_pipe(t_btree *node, t_list **env);
int				open_file(t_tk_type type, char *filename);

int				handle_word(t_btree *node, t_list **env);
char			**get_args(t_btree *node);
char			**lst_to_array(t_list *lst);

int				exec_program(char **argv, char **envp);
char			**get_path(char **envp);
char			*get_program_path(char **path, char *program);
int				check_binary(char *filename);
int				fork_process(char *binary, char **argv, char **envp);

/*
**	BUILT-IN FUNCTIONS
*/

int 			(*is_builtin(char *name))(char **, t_list **);
int				builtin_echo(char **argv, t_list **env);
int				builtin_cd(char **argv, t_list **env);
int				builtin_pwd(char **argv, t_list **env);
int				builtin_export(char **argv, t_list **env);

int				builtin_unset(char **argv, t_list **env);
int				builtin_env(char **argv, t_list **env);
int				builtin_exit(char **argv, t_list **env);
int				how_many_arguments(char **argv);

/*
**	FREE FUNCTIONS
*/

void			free_token(void *ptr);

#endif
