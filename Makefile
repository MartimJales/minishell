#  |  |  ___ \    \  |         |
#  |  |     ) |  |\/ |   _  |  |  /   _
# ___ __|  __/   |   |  (   |    <    __/
#    _|  _____| _|  _| \__,_| _|\_\ \___|
#                              by jcluzet
################################################################################
#                                     CONFIG                                   #
################################################################################

NAME        := minishell
CC        := gcc
FLAGS    := -Wall -Wextra -Werror #-fsanitize="address"
################################################################################
#                                 PROGRAM'S SRCS                               #
################################################################################

SRCS        :=      src/aux.c \
						  src/aux2.c \
						  src/aux3.c \
						  src/aux4.c \
						  src/aux5.c \
						  src/aux6.c \
                          src/main.c \
						  src/cd.c \
						  src/exit.c \
						  src/export.c \
						  src/unset.c \
						  src/heredoc.c \
						  src/redir.c \
						  src/execpipe.c\
                          src/lexer.c \
						  src/llist.c \
						  src/exec.c \
						  src/mit.c \
						  src/lexer2.c \
						  src/lexer3.c \
						  src/lexer4.c \
						  src/tree.c \
						  src/env.c \
						  src/junta_tokens.c \
						  src/dollar.c \
						  src/free.c \



OBJS        := $(SRCS:.c=.o)

.c.o:
	${CC} ${FLAGS} -c -g $< -o ${<:.c=.o}

################################################################################
#                                  Makefile  objs                              #
################################################################################


CLR_RMV		:= \033[0m
RED		    := \033[1;31m
GREEN		:= \033[1;32m
YELLOW		:= \033[1;33m
BLUE		:= \033[1;34m
CYAN 		:= \033[1;36m
RM		    := rm -f

${NAME}:	${OBJS}
			@echo "$(GREEN)Compilation ${CLR_RMV}of ${YELLOW}$(NAME) ${CLR_RMV}..."
			${CC} ${FLAGS} -o ${NAME} ${OBJS} -lreadline
			@echo "$(GREEN)$(NAME) created[0m ✔️"

all:		${NAME}

bonus:		all

clean:
			@ ${RM} *.o */*.o */*/*.o
			@ echo "$(RED)Deleting $(CYAN)$(NAME) $(CLR_RMV)objs ✔️"

fclean:		clean
			@ ${RM} ${NAME}
			@ echo "$(RED)Deleting $(CYAN)$(NAME) $(CLR_RMV)binary ✔️"

re:			fclean all

.PHONY:		all clean fclean re


