#ifndef STRUCT_H
#define STRUCT_H

# include <stdbool.h>

typedef struct x_arg
{
    char  **arg;
    char  **red;
    int     *heredoc_fds;
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
    bool            qout_rm;
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
    int type;
} t_redir;

typedef struct s_pipeline_state //for pip
{
    int prev_input;
    int pipe_fds[2];
    int *pids;
    int num_commands;
    t_env *env;
} t_pipeline_state;

typedef struct s_sig
{
    int             sigint;
    int             sigquit;
    int             exit_status;
    pid_t           pid;
} t_sig;

t_sig g_sig;

#endif