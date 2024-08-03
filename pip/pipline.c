#include "../mini_shell.h"

char **split_pipeline(char *line)
{
    return (ft_split(line, '|'));
}

void redirect_io(int input_fd, int output_fd)
{
    dup2(input_fd, STDIN_FILENO);
    dup2(output_fd, STDOUT_FILENO);
}

int run_pipeline_command(char *command, t_env *env, int input_fd, int output_fd)
{
    char **argv;
    int status;
    int pid;
    int temp_stdin;
    int temp_stdout;
    t_arg temp_cmd;

    argv = split_line(command);
    status = 0;
    pid = 0;
    if (is_builtin(argv[0]))
    {
        temp_stdin = dup(STDIN_FILENO);
        temp_stdout = dup(STDOUT_FILENO);

        redirect_io(input_fd, output_fd);
        temp_cmd.arg = argv;
        status = execute_builtin(&temp_cmd, env, &status);

        dup2(temp_stdin, STDIN_FILENO);
        dup2(temp_stdout, STDOUT_FILENO);
        close(temp_stdin);
        close(temp_stdout);
    }
    else
    {
        pid = fork();
        if (pid == 0)
        {
            redirect_io(input_fd, output_fd);
            if (input_fd != STDIN_FILENO)
                close(input_fd);
            if (output_fd != STDOUT_FILENO)
                close(output_fd);
            status = execute_external_command(argv, env->env_vars);
            exit(status);
        }
    }
    free_argv(argv);
    return pid;
}

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

int handle_pipeline(char **commands, t_env *env)
{
    int pipe_fds[2];
    int prev_input;
    int temp_stdout;
    int pid;
    int status;
    int num_commands;
    int *pids;

    prev_input = STDIN_FILENO;
    temp_stdout = dup(STDOUT_FILENO);
    status = 0;
    num_commands = 0;
    pids = malloc(sizeof(int) * 1024);// Assume max 1024 commands in pipeline
    if (!pids)
    {
        perror("malloc");
        return 1;
    }
    while (*commands)
    {
        if (*(commands + 1))
            pipe(pipe_fds);
        else
            pipe_fds[1] = temp_stdout;
        pid = run_pipeline_command(*commands, env, prev_input, pipe_fds[1]);
        if (pid == -1)
        {
            free(pids);
            return 1;  // Error in run_pipeline_command
        }
        pids[num_commands++] = pid;
        if (prev_input != STDIN_FILENO)
            close(prev_input);
        if (pipe_fds[1] != temp_stdout)
            close(pipe_fds[1]);
        prev_input = pipe_fds[0];
        commands++;
    }
    // Wait for all processes to finish
    for (int i = 0; i < num_commands; i++)
    {
        waitpid(pids[i], &status, 0);
    }
    // Clean up
    if (prev_input != STDIN_FILENO)
        close(prev_input);
    close(temp_stdout);
    free(pids);

    return WEXITSTATUS(status);
}

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