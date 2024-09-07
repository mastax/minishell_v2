/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <elel-bah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:55:40 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/07 13:27:37 by elel-bah         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
    int i = 0;
    while (i < command_count)
    {
        if (waitpid(pids[i], &status, 0) == -1)
        {
            perror("waitpid");
            return 1;
        }
        if (WIFEXITED(status))
            *exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            *exit_status = 128 + WTERMSIG(status);
        if (i == command_count - 1)
            get_exit_status(*exit_status);
        i++;
    }
    return *exit_status;
}


//=-=-=-=-=-

void setup_child_process(child_setup_params *params)
{
    int i;

    if (params->cmd_index > 0)
        dup2(params->pipe_fds[params->cmd_index - 1][0], STDIN_FILENO);
    if (params->cmd_index < params->pipe_count)
        dup2(params->pipe_fds[params->cmd_index][1], STDOUT_FILENO);
    i = 0;
    while (i < params->pipe_count)
    {
        close(params->pipe_fds[i][0]);
        close(params->pipe_fds[i][1]);
        i++;
    }
    if (params->heredoc_fds && params->heredoc_count > 0)
    {
        dup2(params->heredoc_fds[params->heredoc_count - 1], STDIN_FILENO);
        i = 0;
        while (i < params->heredoc_count)
        {
            close(params->heredoc_fds[i]);
            i++;
        }
        // Don't free heredoc_fds here, it will be freed in the parent process
    }
}
