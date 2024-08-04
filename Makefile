NAME = minishell
CC = cc
SRC = builtin/cd.c \
      builtin/echo.c \
      builtin/env.c \
      builtin/exit.c \
      builtin/export.c \
      builtin/pwd.c \
      builtin/unset.c \
      executor/builtin_executer.c \
      executor/cmd_executor.c \
      executor/cmd_executer_utils.c \
      libft/libft_utils.c \
      libft/libft_utils1.c \
      libft/libft_utils2.c \
      libft/libft_utils3.c \
      libft/libft_utils4.c \
      libft/libft_utils5.c \
      builtin/builtin_helpers/cd_helper.c \
      builtin/builtin_helpers/export_helper.c \
      builtin/builtin_helpers/set_env_value.c \
      pip/pipline.c \
      redirections/redirections.c \
      parsing/check_error.c \
      parsing/expanding.c \
      parsing/list_utils.c \
      parsing/parsing.c \
      parsing/remove_quotes.c \
      parsing/token_to_args.c \
      parsing/utils.c \
      parsing/utils1.c \
      parsing/utils2.c \
      parsing/arg_utils.c \
	  mini_shell.c
OBJ = $(SRC:.c=.o)
RM = rm -f
FLAGC = -Wall -Wextra -Werror -g -fsanitize=address
LDFLAGS = -lreadline
HEADER = mini_shell.h

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(FLAGC) -o $(NAME) $(LDFLAGS)

%.o: %.c $(HEADER)
	$(CC) $(FLAGC) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re


