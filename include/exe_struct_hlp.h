#ifndef EXE_STRUCT_HLP_H
#define EXE_STRUCT_HLP_H


typedef struct
{
    int cmd_index;
    int pipe_count;
    int (*pipe_fds)[2];
    int *heredoc_fds;
    int heredoc_count;
} child_setup_params;

typedef struct
{
    t_arg *cmd;
    t_env *env;
    int *exit_status;
    int cmd_index;
    int pipe_count;
    int (*pipe_fds)[2];
    int *heredoc_fds;
    pid_t *pid;
} fork_execute_params;

typedef struct
{
    t_arg *cmd;
    t_env *env;
    int *exit_status;
    int command_count;
    int pipe_count;
    int (*pipe_fds)[2];
    pid_t *pids;
    t_io *io;
} execute_loop_params;

typedef struct
{
    t_arg *cmd;
    t_env *env;
    t_io *io;
    int *heredoc_count;
    int **heredoc_fds;
    int command_count;
} heredoc_setup_params;

typedef struct
{
    t_arg *cmd;
    int *command_count;
    int *pipe_count;
    int (*pipe_fds)[2];
    t_io *io;
} setup_execution_params;

#endif 