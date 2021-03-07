NAME			= minishell
LIBFT			= libft/libft.a
INCLUDE			= ./includes
SRC				= minishell.c environment.c lexer.c parser.c executor.c expander.c free.c	\
					word_execution.c builtins.c builtins2.c program_execution.c pipe.c		\
					redirection.c builtins_utils.c
OBJ_DIR			= objs/
OBJ				= ${addprefix ${OBJ_DIR}, ${SRC:.c=.o}}
CC				= gcc
CFLAGS			= -g -Wall -Wextra #-Werror

${OBJ_DIR}%.o:	srcs/%.c
				${CC} ${CFLAGS} -I ${INCLUDE} -c $< -o $@

${NAME}:		${OBJ_DIR} ${OBJ} ${LIBFT}
				${CC} ${CFLAGS} -o $@ ${OBJ} ${LIBFT}

${OBJ_DIR}:
				mkdir -p ${OBJ_DIR}

${LIBFT}:
				${MAKE} bonus -C libft

all:			${NAME}

clean:
				rm -rf ${OBJ_DIR}
				${MAKE} clean -C libft

fclean:			clean
				rm -rf ${NAME}
				rm -rf ${LIBFT}

re:				fclean all

.PHONY:			all clean fclean re
