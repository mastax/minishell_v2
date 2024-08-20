NAME = minishell
CC = cc
SRC = builtin/cd.c \
      builtin/echo.c \
      builtin/env.c \
      builtin/exit.c \
      builtin/export.c \
      builtin/pwd.c \
      builtin/unset.c \
      builtin/builtin_executer.c \
      executor/cmd_executer_utils.c \
      executor/exe_external_cmd.c \
      executor/execute_command_utils.c \
      executor/execute_command.c \
      executor/find_command.c \
      executor/main_loop.c \
      executor/main.c \
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
      pip/pipline_run.c \
      redirections/redirections.c \
      parsing/check_error.c \
      parsing/expanding.c \
      parsing/expanding_utils.c \
      parsing/list_utils.c \
      parsing/parsing.c \
      parsing/remove_quotes.c \
      parsing/token_to_args.c \
      parsing/utils.c \
      parsing/utils1.c \
      parsing/utils2.c \
      parsing/expand_exit_status.c\
      parsing/arg_utils.c \
      heredoc/heredoc.c \
      heredoc/heredoc_utils.c \
      signals/handle_signals.c \
	  mini_shell.c
OBJ = $(SRC:.c=.o)
RM = rm -f
FLAGC = -Wall -Wextra -Werror -g -fsanitize=address
LDFLAGS = -lreadline
HEADER = mini_shell.h
READLINEDIR = $(shell brew --prefix readline)
LDFLAGS = -L$(READLINEDIR)/lib -lreadline
CPPFLAGS = -I$(READLINEDIR)/include

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(FLAGC) -o $(NAME) $(LDFLAGS)

%.o: %.c $(HEADER)
	$(CC) $(FLAGC) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re


