#include "../mini_shell.h"

// char **split_pipeline(char *line)
// {
//     return (ft_split(line, '|'));
// }

// void redirect_io(int input_fd, int output_fd)
// {
//     dup2(input_fd, STDIN_FILENO);
//     dup2(output_fd, STDOUT_FILENO);
// }

static char **resize_tokens(char **tokens, int *bufsize, int position)
{
    char **new_tokens;
    int new_bufsize = *bufsize + MAX_ARGS;

    new_tokens = malloc(new_bufsize * sizeof(char*));
    if (!new_tokens)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < position; i++)
    {
        new_tokens[i] = tokens[i];
    }
    free(tokens);
    *bufsize = new_bufsize;
    return new_tokens;
}

char **split_line(char *line)
{
    int bufsize = MAX_ARGS;
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    token = ft_strtok(line, DELIMITERS);
    while (token != NULL)
    {
        tokens[position] = ft_strdup(token);
        position++;

        if (position >= bufsize)
        {
            tokens = resize_tokens(tokens, &bufsize, position);
        }
        token = ft_strtok(NULL, DELIMITERS);
    }
    tokens[position] = NULL;
    return tokens;
}

// char **split_line(char *line)
// {
//     int bufsize = MAX_ARGS;
//     int position = 0;
//     char **tokens = malloc(bufsize * sizeof(char*));
//     char *token;

//     if (!tokens)
//     {
//         perror("malloc");
//         exit(EXIT_FAILURE);
//     }

//     token = ft_strtok(line, DELIMITERS);
//     while (token != NULL)
//     {
//         tokens[position] = ft_strdup(token);
//         position++;

//         if (position >= bufsize)
//         {
//             bufsize += MAX_ARGS;
//             tokens = realloc(tokens, bufsize * sizeof(char*));
//             if (!tokens)
//             {
//                 perror("realloc");
//                 exit(EXIT_FAILURE);
//             }
//         }
//         token = ft_strtok(NULL, DELIMITERS);
//     }
//     tokens[position] = NULL;
//     return tokens;
// }

static void setup_builtin_io(int input_fd, int output_fd, int *temp_stdin, int *temp_stdout)
{
    *temp_stdin = dup(STDIN_FILENO);
    *temp_stdout = dup(STDOUT_FILENO);
    redirect_io(input_fd, output_fd);
}

static void restore_builtin_io(int temp_stdin, int temp_stdout)
{
    dup2(temp_stdin, STDIN_FILENO);
    dup2(temp_stdout, STDOUT_FILENO);
    close(temp_stdin);
    close(temp_stdout);
}

int execute_builtin_command(char **argv, t_env *env, int input_fd, int output_fd)
{
    int status = 0;
    int temp_stdin, temp_stdout;
    t_arg temp_cmd;

    setup_builtin_io(input_fd, output_fd, &temp_stdin, &temp_stdout);
    temp_cmd.arg = argv;
    status = execute_builtin(&temp_cmd, env, &status);
    restore_builtin_io(temp_stdin, temp_stdout);

    return status;
}

int execute_external_command_fork(char **argv, t_env *env, int input_fd, int output_fd)
{
    int pid = fork();
    if (pid == 0)
    {
        redirect_io(input_fd, output_fd);
        if (input_fd != STDIN_FILENO)
            close(input_fd);
        if (output_fd != STDOUT_FILENO)
            close(output_fd);
        int status = execute_external_command(argv, env->env_vars);
        exit(status);
    }
    return pid;
}

// int run_pipeline_command(char *command, t_env *env, int input_fd, int output_fd)
// {
//     char **argv = split_line(command);
//     int pid = 0;

//     if (is_builtin(argv[0]))
//     {
//         execute_builtin_command(argv, env, input_fd, output_fd);
//     }
//     else
//     {
//         pid = execute_external_command_fork(argv, env, input_fd, output_fd);
//     }

//     free_argv(argv);
//     return pid;
// }

// int run_pipeline_command(char *command, t_env *env, int input_fd, int output_fd)
// {
//     char **argv;
//     int status;
//     int pid;
//     int temp_stdin;
//     int temp_stdout;
//     t_arg temp_cmd;

//     argv = split_line(command);
//     status = 0;
//     pid = 0;
//     if (is_builtin(argv[0]))
//     {
//         temp_stdin = dup(STDIN_FILENO);
//         temp_stdout = dup(STDOUT_FILENO);

//         redirect_io(input_fd, output_fd);
//         temp_cmd.arg = argv;
//         status = execute_builtin(&temp_cmd, env, &status);

//         dup2(temp_stdin, STDIN_FILENO);
//         dup2(temp_stdout, STDOUT_FILENO);
//         close(temp_stdin);
//         close(temp_stdout);
//     }
//     else
//     {
//         pid = fork();
//         if (pid == 0)
//         {
//             redirect_io(input_fd, output_fd);
//             if (input_fd != STDIN_FILENO)
//                 close(input_fd);
//             if (output_fd != STDOUT_FILENO)
//                 close(output_fd);
//             status = execute_external_command(argv, env->env_vars);
//             exit(status);
//         }
//     }
//     free_argv(argv);
//     return pid;
// }

// int run_pipeline_command(char *command, t_env *env, int input_fd, int output_fd)
// {
//     int pid;
//     int status = 0;

//     pid = fork();
//     if (pid == 0)
//     {
//         redirect_io(input_fd, output_fd);
//         if (input_fd != STDIN_FILENO)
//             close(input_fd);
//         if (output_fd != STDOUT_FILENO)
//             close(output_fd);
//         status = execute_command(command, env);
//         exit(status);
//     }
//     return pid;
// }

//-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


// static void setup_pipe(char **commands, int *pipe_fds, int temp_stdout)
// {
//     if (*(commands + 1))
//         pipe(pipe_fds);
//     else
//         pipe_fds[1] = temp_stdout;
// }

int wait_and_cleanup(int *pids, int num_commands, int prev_input, int temp_stdout)
{
    int status;
    int i;

    i = 0;
    while (i < num_commands)
    {
        waitpid(pids[i], &status, 0);
        i++;
    }
    if (prev_input != STDIN_FILENO)
        close(prev_input);
    close(temp_stdout);
    free(pids);
    return (WEXITSTATUS(status));
}

void handle_command(char *command, t_pipeline_state *state)
{
    int pid;

    pid = run_pipeline_command(command, state->env, state->prev_input, state->pipe_fds[1]);
    if (pid == -1)
    {
        free(state->pids);
        exit(1);
    }
    state->pids[state->num_commands++] = pid;
    if (state->prev_input != STDIN_FILENO)
        close(state->prev_input);
    if (state->pipe_fds[1] != dup(STDOUT_FILENO))
        close(state->pipe_fds[1]);
    state->prev_input = state->pipe_fds[0];
}

// int handle_pipeline(char **commands, t_env *env)
// {
//     t_pipeline_state state;
//     int temp_stdout;

//     state.prev_input = STDIN_FILENO;
//     temp_stdout = dup(STDOUT_FILENO);
//     state.num_commands = 0;
//     state.env = env;
//     state.pids = malloc(sizeof(int) * 1024);
//     if (!state.pids)
//     {
//         perror("malloc");
//         return (1);
//     }
//     while (*commands)
//     {
//         setup_pipe(commands, state.pipe_fds, temp_stdout);
//         handle_command(*commands, &state);
//         commands++;
//     }
//     return (wait_and_cleanup(state.pids, state.num_commands, state.prev_input, temp_stdout));
// }

// int handle_pipeline(char **commands, t_env *env)
// {
//     int pipe_fds[2];
//     int prev_input;
//     int temp_stdout;
//     int pid;
//     int status;
//     int num_commands;
//     int *pids;

//     prev_input = STDIN_FILENO;
//     temp_stdout = dup(STDOUT_FILENO);
//     status = 0;
//     num_commands = 0;
//     pids = malloc(sizeof(int) * 1024);// Assume max 1024 commands in pipeline
//     if (!pids)
//     {
//         perror("malloc");
//         return 1;
//     }
//     while (*commands)
//     {
//         if (*(commands + 1))
//             pipe(pipe_fds);
//         else
//             pipe_fds[1] = temp_stdout;
//         pid = run_pipeline_command(*commands, env, prev_input, pipe_fds[1]);
//         if (pid == -1)
//         {
//             free(pids);
//             return 1;  // Error in run_pipeline_command
//         }
//         pids[num_commands++] = pid;
//         if (prev_input != STDIN_FILENO)
//             close(prev_input);
//         if (pipe_fds[1] != temp_stdout)
//             close(pipe_fds[1]);
//         prev_input = pipe_fds[0];
//         commands++;
//     }
//     // Wait for all processes to finish
//     for (int i = 0; i < num_commands; i++)
//     {
//         waitpid(pids[i], &status, 0);
//     }
//     // Clean up
//     if (prev_input != STDIN_FILENO)
//         close(prev_input);
//     close(temp_stdout);
//     free(pids);

//     return WEXITSTATUS(status);
// }

// int handle_pipeline(char **commands, t_env *env)
// {
//     int pipe_fds[2];
//     int prev_input = STDIN_FILENO;
//     int temp_stdout = dup(STDOUT_FILENO);
//     int pid, last_pid = 0;
//     int status = 0;

//     while (*commands)
//     {
//         if (*(commands + 1))
//             pipe(pipe_fds);
//         else
//             pipe_fds[1] = temp_stdout;
//         pid = run_pipeline_command(*commands, env, prev_input, pipe_fds[1]);
//         if (prev_input != STDIN_FILENO)
//             close(prev_input);
//         if (pipe_fds[1] != temp_stdout)
//             close(pipe_fds[1]);
//         prev_input = pipe_fds[0];
//         last_pid = pid;
//         commands++;
//     }
//     waitpid(last_pid, &status, 0);
//     if (prev_input != STDIN_FILENO)
//         close(prev_input);
//     close(temp_stdout);
//     return WEXITSTATUS(status);
// }
