#include "mini_shell.h"

static int execute_commands(t_arg *cmd, t_env *env, int *exit_status, int command_count, int pipe_count, int pipe_fds[][2], pid_t *pids)
{
    (void)command_count;
    int cmd_index = 0;

    while (cmd)
    {
        int *heredoc_fds = setup_heredocs(cmd, env);
        if (heredoc_fds == NULL && count_heredocs(cmd->red) > 0) {
            return 1;
        }

        if (fork_and_execute(cmd, env, exit_status, cmd_index, pipe_count, pipe_fds, heredoc_fds, &pids[cmd_index]) != 0) {
            return 1;
        }

        cleanup_parent_process(heredoc_fds, count_heredocs(cmd->red));

        cmd = cmd->next;
        cmd_index++;
    }
    return 0;
}