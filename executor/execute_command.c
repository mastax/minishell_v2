/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <elel-bah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:55:48 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/07 13:27:47 by elel-bah         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../mini_shell.h"


void track_fd(t_fd_tracker *tracker, int fd)
{
    if (fd >= 0 && fd < MAX_FD)
        tracker->fd_table[fd] = 1;
}

void untrack_fd(t_fd_tracker *tracker, int fd)
{
    if (fd >= 0 && fd < MAX_FD)
        tracker->fd_table[fd] = 0;
}

void close_all_fds(t_fd_tracker *tracker)
{
    int fd;

    fd = 3;
    while (fd < MAX_FD)
    {
        if (tracker->fd_table[fd])
        {
            close(fd);
            untrack_fd(tracker, fd);
        }
        fd++;
    }
}

//=-=-=-=-

// typedef struct s_setup_context {
//     t_arg *cmd;
//     t_env *env;
//     t_io *io;
//     int (*pipe_fds)[2];
//     int *command_count;
//     int *pipe_count;
// } t_setup_context;


int setup_and_handle_heredocs(t_setup_context *ctx)
{
    t_fd_tracker fd_tracker = {0};
    t_arg *current_cmd;
    int heredoc_count;

    *(ctx->command_count) = count_commands(ctx->cmd);
    *(ctx->pipe_count) = *(ctx->command_count) - 1;
    save_original_io(ctx->io);
    if (setup_pipes(*(ctx->pipe_count), ctx->pipe_fds, &fd_tracker) != 0)
    {
        restore_io(ctx->io);
        return 1;
    }
    // Process all heredocs
    current_cmd = ctx->cmd;
    while (current_cmd) {
        heredoc_count = count_heredocs(current_cmd->red);
        if (heredoc_count > 0)
        {
            current_cmd->heredoc_fds = handle_heredocs(current_cmd->red, heredoc_count, ctx->env, &fd_tracker);
            if (!current_cmd->heredoc_fds)
            {
                restore_io(ctx->io);
                return 1;
            }
        }
        current_cmd = current_cmd->next;
    }
    return 0;
}

//=-=-=-=-

// int setup_and_handle_heredocs(t_arg *cmd, t_env *env, t_io *io, int pipe_fds[MAX_PIPES][2], int *command_count, int *pipe_count) {//last one
// 	*command_count = count_commands(cmd);
// 	*pipe_count = *command_count - 1;

// 	save_original_io(io);

// 	if (setup_pipes(*pipe_count, pipe_fds) != 0) {
// 		restore_io(io);
// 		return 1;
// 	}

// 	// Process all heredocs
// 	t_arg *current_cmd = cmd;
// 	while (current_cmd) {
// 		int heredoc_count = count_heredocs(current_cmd->red);
// 		if (heredoc_count > 0) {
// 			current_cmd->heredoc_fds = handle_heredocs(current_cmd->red, heredoc_count, env);
// 			if (!current_cmd->heredoc_fds) {
// 				restore_io(io);
// 				return 1;
// 			}
// 		}
// 		current_cmd = current_cmd->next;
// 	}

// 	return 0;
// }

typedef struct s_command_context1 {
    t_arg *cmd;
    t_env *env;
    int *exit_status;
    t_io *io;
    int (*pipe_fds)[2];
    int command_count;
    int pipe_count;
} t_command_context1;


//=-=-=-=-=-

static void configure_child_io(t_command_context1 *ctx, t_arg *current_cmd, int cmd_index)
{
    if (cmd_index > 0)
        dup2(ctx->pipe_fds[cmd_index - 1][0], STDIN_FILENO);
    if (cmd_index < ctx->pipe_count)
        dup2(ctx->pipe_fds[cmd_index][1], STDOUT_FILENO);
    for (int i = 0; i < ctx->pipe_count; i++)
    {
        close(ctx->pipe_fds[i][0]);
        close(ctx->pipe_fds[i][1]);
    }
    if (current_cmd->heredoc_fds)
    {
        for (int i = 0; i < count_heredocs(current_cmd->red); i++)
        {
            dup2(current_cmd->heredoc_fds[i], STDIN_FILENO);
            close(current_cmd->heredoc_fds[i]);
        }
    }
    if (apply_redirections(current_cmd->red) == -1)
        exit(1);
}

static void execute_child_process(t_command_context1 *ctx, t_arg *current_cmd)
{
    if (current_cmd->arg == NULL || current_cmd->arg[0] == NULL)
        exit(0);
    if (is_builtin(current_cmd->arg[0]))
    {
        int builtin_result = execute_builtin_ch(current_cmd, ctx->env, ctx->exit_status);
        exit(builtin_result);
    }
    if (current_cmd->arg[0][0] == '\0')
    {
        ft_putstr_fd(": command not found\n", 2);
        exit(127);
    }
    char *cmd_path = ft_get_path(current_cmd->arg[0], ctx->exit_status);
    if (cmd_path == NULL)
        cmd_path = find_command(current_cmd->arg[0], ctx->env->env_vars);
    if (!cmd_path) {
        ft_putstr_fd(current_cmd->arg[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        exit(127);
    }
    execve(cmd_path, current_cmd->arg, ctx->env->env_vars);
    perror("execve");
    exit(1);
}

static int handle_parent_process(t_command_context1 *ctx, t_arg *current_cmd, pid_t *pids, int cmd_index)
{
    pids[cmd_index] = fork();
    if (pids[cmd_index] == -1)
    {
        perror("fork");
        restore_io(ctx->io);
        return 0;
    }
    else if (pids[cmd_index] == 0)
    {
        configure_child_io(ctx, current_cmd, cmd_index);
        execute_child_process(ctx, current_cmd);
    }
    restore_io(ctx->io);
    return 1;
}
//=-=-=-=-

static void handle_parent_builtin(t_command_context1 *ctx, t_arg *current_cmd)
{
    if (apply_redirections(current_cmd->red) == -1)
    {
        restore_io(ctx->io);
        return;
    }
    execute_builtin_p(current_cmd, ctx->env, ctx->exit_status);
    restore_io(ctx->io);
}

static int process_commands(t_command_context1 *ctx, pid_t *pids)
{
    int child_count = 0;
    int cmd_index = 0;
    t_arg *current_cmd = ctx->cmd;

    while (current_cmd)
    {
        int is_builtins = current_cmd->arg != NULL && is_builtin(current_cmd->arg[0]);
        int is_parent_builtin = is_builtins && (ft_strcmp(current_cmd->arg[0], "cd") == 0 ||
                                                ft_strcmp(current_cmd->arg[0], "exit") == 0 ||
                                                ft_strcmp(current_cmd->arg[0], "export") == 0 ||
                                                ft_strcmp(current_cmd->arg[0], "unset") == 0);

        if (is_parent_builtin && ctx->pipe_count == 0)
            handle_parent_builtin(ctx, current_cmd);
        else
            child_count += handle_parent_process(ctx, current_cmd, pids, cmd_index);

        get_pid(pids[cmd_index]);
        current_cmd = current_cmd->next;
        cmd_index++;
    }
    return child_count;
}

static void cleanup_pipes(t_command_context1 *ctx)
{
    for (int i = 0; i < ctx->pipe_count; i++) {
        close(ctx->pipe_fds[i][0]);
        close(ctx->pipe_fds[i][1]);
    }
}

static void handle_parent_builtin(t_command_context1 *ctx, t_arg *current_cmd);
static int process_commands(t_command_context1 *ctx, pid_t *pids);
static void cleanup_pipes(t_command_context1 *ctx);

int execute_commands(t_command_context1 *ctx)
{
    pid_t pids[MAX_COMMANDS];
    int child_count = process_commands(ctx, pids);
    
    cleanup_pipes(ctx);
    
    int result = 0;
    if (child_count > 0)
        result = wait_for_children(pids, child_count, ctx->exit_status);
    restore_io(ctx->io);
    return result;
}

void cleanup_command_heredocs(t_arg *cmd, t_fd_tracker *fd_tracker) {
    int i;
    int heredoc_count;

    i = 0;
    heredoc_count = count_heredocs(cmd->red);
    while (i < heredoc_count) {
        if (cmd->heredoc_fds[i] > 2)
        {
            close(cmd->heredoc_fds[i]);
            untrack_fd(fd_tracker, cmd->heredoc_fds[i]);
        }
        i++;
    }
}

void cleanup_heredoc_fds(t_arg *cmd, t_fd_tracker *fd_tracker)
{
    t_arg *current_cmd;

    current_cmd = cmd;
    while (current_cmd)
    {
        if (current_cmd->heredoc_fds) {
            cleanup_command_heredocs(current_cmd, fd_tracker);
        }
        current_cmd = current_cmd->next;
    }
}

//=-=-=-=-=-=-=-

static void init_setup_context(t_setup_context *setup_ctx, t_command_context *context, 
                               t_execution_data *exec_data)
{
    setup_ctx->cmd = context->cmd;
    setup_ctx->env = context->env;
    setup_ctx->io = &exec_data->io;
    setup_ctx->pipe_fds = exec_data->pipe_fds;
    setup_ctx->command_count = &exec_data->command_count;
    setup_ctx->pipe_count = &exec_data->pipe_count;
}

static void init_command_context1(t_command_context1 *ctx, t_command_context *context, 
                                  t_execution_data *exec_data)
{
    ctx->cmd = context->cmd;
    ctx->env = context->env;
    ctx->exit_status = context->exit_status;
    ctx->io = &exec_data->io;
    ctx->pipe_fds = exec_data->pipe_fds;
    ctx->command_count = exec_data->command_count;
    ctx->pipe_count = exec_data->pipe_count;
}

int execute_command(t_command_context *context)
{
    t_execution_data exec_data = {0};
    t_setup_context setup_ctx;
    t_command_context1 cmd_ctx;
    int result;

    exec_data.command_count = 0;
    exec_data.pipe_count = 0;

    init_setup_context(&setup_ctx, context, &exec_data);

    if (setup_and_handle_heredocs(&setup_ctx) != 0)
    {
        close_all_fds(context->fd_tracker);
        return (1);
    }
    init_command_context1(&cmd_ctx, context, &exec_data);
    result = execute_commands(&cmd_ctx);
    cleanup_heredoc_fds(context->cmd, context->fd_tracker);
    return (result);
}
