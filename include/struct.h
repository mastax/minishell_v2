#ifndef STRUCT_H
#define STRUCT_H

typedef struct x_arg
{
    char  **arg;
    char  **red;
    int     fd;// for heredoc
    struct x_arg *next;
} t_arg;

typedef enum s_type
{
	WORD,
	PIPE,
	RED_IN,
	RED_OUT,
	HER_DOC,
	APPEND,
}	t_type;

typedef struct s_tokenz
{
	char			*content;
	int				type;
	struct s_tokenz	*next;
}	t_token;

// Add this to your header file (mini_shell.h)
typedef struct s_env
{
    char    **env_vars;
    int     count;
} t_env;

typedef struct s_io
{
    int original_stdin;
    int original_stdout;
} t_io;

typedef struct s_redir
{
    int fd;
    int original_fd;
    int is_output;
} t_redir;

#endif