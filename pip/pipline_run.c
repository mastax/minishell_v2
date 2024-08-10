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


static void setup_pipe(char **commands, int *pipe_fds, int temp_stdout)
{
    if (*(commands + 1))
        pipe(pipe_fds);
    else
        pipe_fds[1] = temp_stdout;
}

int run_pipeline_command(char *command, t_env *env, int input_fd, int output_fd)
{
    char **argv = split_line(command);
    int pid = 0;

    if (is_builtin(argv[0]))
    {
        execute_builtin_command(argv, env, input_fd, output_fd);
    }
    else
    {
        pid = execute_external_command_fork(argv, env, input_fd, output_fd);
    }

    free_argv(argv);
    return pid;
}

int handle_pipeline(char **commands, t_env *env)
{
    t_pipeline_state state;
    int temp_stdout;

    state.prev_input = STDIN_FILENO;
    temp_stdout = dup(STDOUT_FILENO);
    state.num_commands = 0;
    state.env = env;
    state.pids = malloc(sizeof(int) * 1024);
    if (!state.pids)
    {
        perror("malloc");
        return (1);
    }
    while (*commands)
    {
        setup_pipe(commands, state.pipe_fds, temp_stdout);
        handle_command(*commands, &state);
        commands++;
    }
    return (wait_and_cleanup(state.pids, state.num_commands, state.prev_input, temp_stdout));
}
