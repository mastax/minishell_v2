#include "../mini_shell.h"

void cleanup_parent_process(int *heredoc_fds, int heredoc_count)
{
    if (heredoc_fds) {
        for (int i = 0; i < heredoc_count; i++)
            close(heredoc_fds[i]);
        free(heredoc_fds);
    }
}

int count_commands(t_arg *cmd)
{
    int count = 0;
    while (cmd)
    {
        count++;
        cmd = cmd->next;
    }
    return count;
}

int wait_for_children(pid_t *pids, int command_count, int *exit_status)
{
    int status;
    for (int i = 0; i < command_count; i++) {
        if (waitpid(pids[i], &status, 0) == -1) {
            perror("waitpid");
            return 1;
        }
        if (WIFEXITED(status)) {
            *exit_status = WEXITSTATUS(status);
        }
        else if (WIFSIGNALED(status)) {
            *exit_status = 128 + WTERMSIG(status);
        }
        if (i == command_count - 1) {
            g_sig.exit_status = *exit_status;
        }
    }
    return *exit_status;
}


// int wait_for_children(pid_t *pids, int command_count, int *exit_status)
// {
//     for (int i = 0; i < command_count; i++) {
//         int status;
//         waitpid(pids[i], &status, 0);
//         if (i == command_count - 1) {
//             *exit_status = WEXITSTATUS(status);
//             g_sig.exit_status = *exit_status;
//         }
//     }
//     return *exit_status;
// }

void setup_child_process(child_setup_params *params) {
    if (params->cmd_index > 0) {
        dup2(params->pipe_fds[params->cmd_index - 1][0], STDIN_FILENO);
    }
    if (params->cmd_index < params->pipe_count) {
        dup2(params->pipe_fds[params->cmd_index][1], STDOUT_FILENO);
    }

    for (int i = 0; i < params->pipe_count; i++) {
        close(params->pipe_fds[i][0]);
        close(params->pipe_fds[i][1]);
    }

    if (params->heredoc_fds && params->heredoc_count > 0) {
        dup2(params->heredoc_fds[params->heredoc_count - 1], STDIN_FILENO);
        for (int i = 0; i < params->heredoc_count; i++)
            close(params->heredoc_fds[i]);
        // Don't free heredoc_fds here, it will be freed in the parent process
    }
}

int fork_and_execute(fork_execute_params *params) {
    *(params->pid) = fork();
    if (*(params->pid) == -1) {
        perror("fork");
        return 1;
    } else if (*(params->pid) == 0) { // Child process
        signal(SIGINT, SIG_DFL);  // Reset SIGINT to default in child
        signal(SIGQUIT, SIG_DFL);  // Reset SIGQUIT to default in child
        int heredoc_count = count_heredocs(params->cmd->red);
        child_setup_params setup_params = {
            params->cmd_index, params->pipe_count, params->pipe_fds,
            params->heredoc_fds, heredoc_count
        };
        setup_child_process(&setup_params);
        if (apply_redirections(params->cmd->red) == -1)
            exit(1);
        if (params->cmd->arg == NULL)
            exit (0);
        if (is_builtin(params->cmd->arg[0]))
            exit(execute_builtin(params->cmd, params->env, params->exit_status));
        else
            exit(execute_external_command(params->cmd->arg, params->env->env_vars));
    }
    return 0;
}
