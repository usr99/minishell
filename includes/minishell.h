/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 14:33:29 by mamartin          #+#    #+#             */
/*   Updated: 2021/03/18 22:37:03 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "headers.h"
# include "defines.h"
# include "types.h"

t_global		g_global;

/*
**	MINISHELL FUNCTIONS
*/

int				shell_loop(t_env *vars);
void			handle_signal(int signal);
void			print_prompt(void);
void			print_error(char *message, char *name, char *cmd);

int				execute(char *cmd, t_env *vars);
void			handle_signal_code(int signal);

/*
**	ENVIRONMENT VARIABLES FUNCTIONS
*/

t_list			*dup_env(char **env);
char			*get_environment_var(t_list *env, char *name, int *len);
void			sort_alpha_lst(t_list **lst);
int				get_varname_length(char *var);

/*
**	SYNTAX FUNCTIONS
*/

int				check_syntax(char *cmd);
int				check_quotes(char *cmd, int *i);
int				check_operator(char *cmd, int *i);
int				is_word(char c);

/*
**	LEXER FUNCTIONS
*/

t_btree			*read_cmd(char *cmd_line);
t_list			*get_token(char *cmd, int *pos);
t_tk_type		get_token_type(char *cmd);
char			*get_token_data(char *cmd);
t_list			*create_token_link(t_tk_type type, char *data);

/*
**	PARSER FUNCTIONS
*/

t_btree			*parser(t_list *lexer);
int				create_ast(t_btree **node, t_list *lexer);
t_token			*get_highest_token(t_list *lexer, int *index);
int				get_token_rank(t_token *token);
int				is_operator(t_token *token);

/*
**	EXPANDER FUNCTIONS
*/

int				expander(t_btree *ast, t_list *env);
int				expand(t_token *token, t_list *env);
char			*expand_dollar_sign(char *data, t_list *env, int *pos);
char			*replace_env_var(char *data, char *var, int i, int length);
char			*expand_backslash(char c);

int				expand_single_quotes(t_token *token, int *pos);
char			*remove_squotes(t_token *token, int *index, int i);
int				expand_double_quotes(t_token *token, t_list *env, int *pos,
					int size);
char			*dquotes_expansion(t_token *token, t_list *env, int *i);
char			*join_data(t_token *token, int *index, char *new);

char			*create_str(char c);
char			*get_var(t_list *env, char *data, int *i);
int				*get_quotes_index(int *index, char *data, char quote, int i);
int				set_new_data(t_token *token, char *new);

/*
**	EXECUTOR FUNCTIONS
*/

int				exec_ast(t_btree *ast, t_env *vars, int *errcode);
int				exec_node(t_btree *root, t_btree *node, t_env *vars,
					int *errcode);

int				handle_redirect(t_btree *root, t_btree *node, t_env *vars);
int				open_file(t_token *token, int *std_fd);
int				my_dup(int fd1, int fd2, t_token *token);
int				close_dup(int fd, int save, int std, t_token *token);

int				handle_pipe(t_btree *root, t_btree *node, t_env *vars);
int				pipe_child(t_ast ast, t_env *vars, int pipe[2]);
int				pipe_parent(t_ast ast, t_env *vars, int pipe[2], int pid);

int				handle_word(t_btree *root, t_btree *node, t_env *vars);
char			**get_args(t_btree *node);
char			**lst_to_array(t_list *lst);
char			**get_path(char **envp);

int				exec_program(char **argv, char **envp);
char			*get_program_path(char **path, char *program);
int				check_binary(char *filename);
int				fork_process(char *binary, char **argv, char **envp);
int				exec_in_child(char *binary, char **argv, char **envp);

/*
**	BUILT-IN FUNCTIONS
*/

int				builtin_echo(char **argv);
int				builtin_cd(char **argv);
int				builtin_pwd(void);
int				builtin_export(char **argv, t_env *vars);

int				builtin_unset(char **argv, t_env *vars);
int				builtin_env(char **argv, t_list **env);
int				builtin_exit(t_btree *root, char **argv, t_env *vars);

int				is_builtin(t_btree *root, char **argv, t_env *vars);
int				how_many_arguments(char **argv);
int				get_pwd(char **cwd);
void			delete_var(char *argv, t_list **env);
int				create_var(char *arg, t_list *env);

int				check_existing_var(char *str, t_list *env);
int				is_echo_option(char *str);
int				check_numeric_value(char **argv, int nb_args);
void			print_export(t_list *export);

/*
**	FREE FUNCTIONS
*/

void			free_token(void *ptr);
void			destroy_all(t_btree *root, char **argv, t_env *vars);

#endif
