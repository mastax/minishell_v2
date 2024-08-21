int execute_command(t_arg *cmd, t_env *env, int *exit_status) {// the last one before splite:
    t_io io;
    int command_count, pipe_count;
    int pipe_fds[MAX_PIPES][2];
    pid_t pids[MAX_COMMANDS];
    int child_count = 0;

    command_count = count_commands(cmd);
    pipe_count = command_count - 1;

    save_original_io(&io);

    if (setup_pipes(pipe_count, pipe_fds) != 0) {
        restore_io(&io);
        return 1;
    }

    // First, process all heredocs
    t_arg *current_cmd = cmd;
    while (current_cmd) {
        int heredoc_count = count_heredocs(current_cmd->red);
        if (heredoc_count > 0) {
            current_cmd->heredoc_fds = handle_heredocs(current_cmd->red, heredoc_count, env);
            if (!current_cmd->heredoc_fds) {
                restore_io(&io);
                return 1;
            }
        }
        current_cmd = current_cmd->next;
    }

    // Now execute commands
    current_cmd = cmd;
    int cmd_index = 0;
    while (current_cmd) {
        if (current_cmd->arg != NULL && is_builtin(current_cmd->arg[0])) {
            // Execute the builtin in the parent process
            execute_builtin(current_cmd, env, exit_status);
        } else {
            pids[cmd_index] = fork();
            if (pids[cmd_index] == -1) {
                perror("fork");
                restore_io(&io);
                return 1;
            } else if (pids[cmd_index] == 0) { // Child process
                // Set up pipes, heredoc, and other redirections
                if (cmd_index > 0) {
                    dup2(pipe_fds[cmd_index - 1][0], STDIN_FILENO);
                }
                if (cmd_index < pipe_count) {
                    dup2(pipe_fds[cmd_index][1], STDOUT_FILENO);
                }
                for (int i = 0; i < pipe_count; i++) {
                    close(pipe_fds[i][0]);
                    close(pipe_fds[i][1]);
                }
                if (current_cmd->heredoc_fds) {
                    for (int i = 0; i < count_heredocs(current_cmd->red); i++) {
                        dup2(current_cmd->heredoc_fds[i], STDIN_FILENO);
                        close(current_cmd->heredoc_fds[i]);
                    }
                }
                // Execute the command
                char *cmd_path = find_command(current_cmd->arg[0], env->env_vars);
                if (!cmd_path) {
                    ft_putstr_fd(current_cmd->arg[0], 2);
                    ft_putstr_fd(": command not found\n", 2);
                    exit(127);
                }
                execve(cmd_path, current_cmd->arg, env->env_vars);
                perror("execve");
                exit(1);
            }
            child_count++;
        }
        g_sig.pid = pids[cmd_index];
        current_cmd = current_cmd->next;
        cmd_index++;
    }

    // Close all pipe fds in parent
    for (int i = 0; i < pipe_count; i++) {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
    }

    // Wait for all child processes
    int result = 0;
    if (child_count > 0) {
        result = wait_for_children(pids, child_count, exit_status);
    }

    restore_io(&io);
    return result;
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

void save_original_io(t_io *io)
{
    io->original_stdin = dup(STDIN_FILENO);
    io->original_stdout = dup(STDOUT_FILENO);
}

int setup_pipes(int pipe_count, int pipe_fds[][2])
{
    for (int i = 0; i < pipe_count; i++) {
        if (pipe(pipe_fds[i]) == -1) {
            perror("pipe");
            return 1;
        }
    }
    return 0;
}

void restore_io(t_io *io)
{
    dup2(io->original_stdin, STDIN_FILENO);
    dup2(io->original_stdout, STDOUT_FILENO);
    close(io->original_stdin);
    close(io->original_stdout);
}

int count_heredocs(char **red)
{
    int count = 0;
    int i = 0;
    if (!red || !(*red))
        return (0);
    while (red[i] != NULL)
    {
        if (ft_strcmp(red[i], "<<") == 0)
            count++;
        i++;
    }
    return count;
}

int *handle_heredocs(char **red, int count, t_env *env)
{
    int *heredoc_fds = malloc(sizeof(int) * count);
    if (!heredoc_fds)
        return NULL;

    for (int i = 0; i < count; i++)
    {
        char *delimiter = get_heredoc_delimiter(red, i);
        heredoc_fds[i] = create_heredoc(delimiter, env);
        if (heredoc_fds[i] == -1)
        {
            // Clean up previously created here-docs
            for (int j = 0; j < i; j++)
                close(heredoc_fds[j]);
            free(heredoc_fds);
            return NULL;
        }
    }
    return (heredoc_fds);
}

int is_builtin(const char *cmd)
{
    const char *builtins[] = {
        "echo", "cd", "pwd", "export", "unset", "env", "exit", NULL
    };
    if (cmd == NULL)
        return (0);
    for (int i = 0; builtins[i]; i++)
    {
        if (ft_strcmp(cmd, builtins[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int execute_builtin(t_arg *cmd, t_env *env, int *exit_status)
{
    if (cmd->arg[0] == NULL)
        return (0);
    if (strcmp(cmd->arg[0], "cd") == 0)
        return ft_change_dir(cmd->arg, env);
    else if (strcmp(cmd->arg[0], "echo") == 0)
        return ft_echo(cmd->arg);
    else if (strcmp(cmd->arg[0], "env") == 0)
        return ft_env(env);
    else if (strcmp(cmd->arg[0], "exit") == 0)
        return ft_exit(cmd->arg, *exit_status);
    else if (strcmp(cmd->arg[0], "export") == 0)
        return ft_exports(env, cmd->arg, exit_status);
    else if (strcmp(cmd->arg[0], "pwd") == 0)
        return pwd(STDOUT_FILENO);
    else if (strcmp(cmd->arg[0], "unset") == 0)
        return ft_unsets(env, cmd->arg, exit_status);
    return 1; // Command not found
}

char *find_command(char *cmd, char **envp)
{
    char *path;
    char *path_copy;
    char *full_path;

    path = get_path_from_env(envp);
    if (!path)
        return (NULL);
    path_copy = ft_strdup(path);
    if (!path_copy)
        return (NULL);
    full_path = search_command_in_path(path_copy, cmd);
    free(path_copy);
    return (full_path);
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
