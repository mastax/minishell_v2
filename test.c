#include "mini_shell.h"

void sig_int(int code)
{
    (void)code;
    if (g_sig.pid == 0)
    {
        write(STDERR_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
    else
    {
        write(STDERR_FILENO, "\n", 1);
    }
    g_sig.sigint = 1;
    g_sig.exit_status = 1;
    // g_sig.exit_status = 130;
}

void sig_quit(int code)
{
    char *nbr;
    nbr = ft_itoa(code);
    if (g_sig.pid != 0)
    {
        ft_putstr_fd("Quit: \n", STDERR_FILENO);
        ft_putendl_fd(nbr, STDERR_FILENO);
        g_sig.exit_status = 131;
        g_sig.sigquit = 1;
    }
    // else
    //     ft_putstr_fd("\b\b  \b\b", STDERR_FILENO);
    free(nbr);
}

void sig_init(void)
{
    g_sig.sigint = 0;
    g_sig.sigquit = 0;
    g_sig.pid = 0;
    g_sig.exit_status = 0;
}

void free_env(t_env *env)
{
    int i;

    i = 0;
    if (!env)
        return;
    while (i < env->count)
    {
        free(env->env_vars[i]);
        i++;
    }
    free(env->env_vars);
    free(env);
}

char *get_env_value(t_env *env, const char *key) //helper cd
{
    int key_len;
    int i;

    key_len = strlen(key);
    i = 0;
    while (i < env->count)
    {
        if (strncmp(env->env_vars[i], key, key_len) == 0 && env->env_vars[i][key_len] == '=')
            return env->env_vars[i] + key_len + 1;
        i++;
    }
    return NULL;
}

void free_argv(char **av)
{
    int i;

    i = 0;
    while (av[i])
    {
        free(av[i]);
        i++;
    }
    free(av);
}

int is_builtin(const char *cmd)
{
    const char *builtins[] = {
        "echo", "cd", "pwd", "export", "unset", "env", "exit", NULL
    };

    for (int i = 0; builtins[i]; i++)
    {
        if (ft_strcmp(cmd, builtins[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int count_heredocs(char **red)
{
    int count = 0;
    int i = 0;
    while (red && red[i])
    {
        if (ft_strcmp(red[i], "<<") == 0)
            count++;
        i++;
    }
    return count;
}

static char *get_command_path(char **argv, char **envp)
{
    char *cmd_path;

    if (!argv[0] || argv[0][0] == '\0')
        return NULL;
    if (access(argv[0], X_OK) == 0)
        return (argv[0]);
    cmd_path = find_command(argv[0], envp);
    if (!cmd_path)
    {
        ft_putstr_fd(argv[0], 2);
        ft_putstr_fd(": command not found\n", 2);
    }
    return (cmd_path);
}

static int execute_child_process(char *cmd_path, char **argv, char **envp)
{
    if (execve(cmd_path, argv, envp) == -1)
    {
        perror("execve");
        exit(1);
    }
    return (0);
}

static int handle_parent_process(pid_t pid)
{
    int status;

    if (waitpid(pid, &status, 0) == -1)
    {
        perror("waitpid");
        return (1);
    }
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        return (128 + WTERMSIG(status));
    return (0);
}

int execute_external_command(char **argv, char **envp)
{
    pid_t pid;
    char *cmd_path;
    int status = 0;

    // Check if the command is an empty string
    if (argv[0] == NULL || argv[0][0] == '\0')
    {
        ft_putstr_fd(": command not found\n", 2);
        return (127);  // Command not found
    }
    cmd_path = get_command_path(argv, envp);
    if (!cmd_path)
        return (127);  // Command not found
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return (1);
    }
    else if (pid == 0)
    {
        if (execute_child_process(cmd_path, argv, envp) == -1)
        {
            perror("execve");
            exit(1);
        }
    }
        //status = execute_child_process(cmd_path, argv, envp);
    else
        status = handle_parent_process(pid);
    if (cmd_path != argv[0])
        free(cmd_path);
    return (status);
}

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
    for (int i = 0; i < command_count; i++) {
        int status;
        waitpid(pids[i], &status, 0);
        if (i == command_count - 1) {
            *exit_status = WEXITSTATUS(status);
            g_sig.exit_status = *exit_status;
        }
    }
    return *exit_status;
}

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

        if (is_builtin(params->cmd->arg[0])) {
            exit(execute_builtin(params->cmd, params->env, params->exit_status));
        } else {
            exit(execute_external_command(params->cmd->arg, params->env->env_vars));
        }
    }
    return 0;
}

static void cleanup_pipes(int pipe_count, int pipe_fds[][2])
{
    for (int i = 0; i < pipe_count; i++) {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
    }
}
//=-=-=-=-=

static int setup_execution(setup_execution_params *params) {
    *(params->command_count) = count_commands(params->cmd);
    *(params->pipe_count) = *(params->command_count) - 1;

    save_original_io(params->io);

    if (setup_pipes(*(params->pipe_count), params->pipe_fds) != 0) {
        restore_io(params->io);
        return 1;
    }
    return 0;
}

static int setup_and_check_heredoc(heredoc_setup_params *params) {
    (void)(params->command_count);
    *(params->heredoc_count) = count_heredocs(params->cmd->red);
    *(params->heredoc_fds) = NULL;
    if (*(params->heredoc_count) > 0)
    {
        *(params->heredoc_fds) = handle_heredocs(params->cmd->red, *(params->heredoc_count), params->env);
        if (!*(params->heredoc_fds)) {
            restore_io(params->io);
            return 1;
        }
    }
    return 0;
}

int execute_commands_loop(execute_loop_params *params) {
    int cmd_index = 0;
    while (params->cmd) {
        int heredoc_count;
        int *heredoc_fds;

        // In execute_commands_loop function:
        heredoc_setup_params heredoc_params = {
        params->cmd, params->env, params->io, &heredoc_count, &heredoc_fds, params->command_count
        };
        int setup_result = setup_and_check_heredoc(&heredoc_params);
        // int setup_result = setup_and_check_heredoc(params->cmd, params->env, params->io, &heredoc_count, &heredoc_fds, params->command_count);
        if (setup_result != 0)
            return setup_result;

        fork_execute_params fork_params = {
            params->cmd, params->env, params->exit_status, cmd_index,
            params->pipe_count, params->pipe_fds, heredoc_fds, &(params->pids[cmd_index])
        };
        if (fork_and_execute(&fork_params) != 0) {
            restore_io(params->io);
            return 1;
        }

        cleanup_parent_process(heredoc_fds, heredoc_count);
        params->cmd = params->cmd->next;
        cmd_index++;
    }
    return 0;
}
//=-=-=--=

//=-=-=-=
int execute_command(t_arg *cmd, t_env *env, int *exit_status) {
    t_io io;
    int command_count, pipe_count;
    int pipe_fds[MAX_PIPES][2];
    pid_t pids[MAX_COMMANDS];

    setup_execution_params setup_params = {
        cmd, &command_count, &pipe_count, pipe_fds, &io
    };
    if (setup_execution(&setup_params) != 0) {
        return 1;
    }

    int cmd_index = 0;
    while (cmd)
    {
        int heredoc_count = count_heredocs(cmd->red);
        int *heredoc_fds = NULL;
        if (heredoc_count > 0)
        {
            heredoc_fds = handle_heredocs(cmd->red, heredoc_count, env);
            if (!heredoc_fds)
            {
                restore_io(&io);
                return 1;
            }
        }
        else if (command_count == 1 && is_builtin(cmd->arg[0])) {
            if (apply_redirections(cmd->red) == -1) {
                restore_io(&io);
                return 1;
            }
            int result = execute_builtin(cmd, env, exit_status);
            restore_io(&io);
            return result;
        }

        fork_execute_params fork_params = {
            cmd, env, exit_status, cmd_index, pipe_count, pipe_fds, heredoc_fds, &pids[cmd_index]
        };
        if (fork_and_execute(&fork_params) != 0) {
            restore_io(&io);
            return 1;
        }

        g_sig.pid = pids[cmd_index];////////

        cleanup_parent_process(heredoc_fds, heredoc_count);

        cmd = cmd->next;
        cmd_index++;
    }

    cleanup_pipes(pipe_count, pipe_fds);

    int result = wait_for_children(pids, command_count, exit_status);

    restore_io(&io);
    return result;
}

static char *get_path_from_env(char **envp)
{
    int i;

    i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
            return (envp[i] + 5);
        i++;
    }
    return (NULL);
}

static char *create_full_path(char *dir, char *cmd)
{
    char *full_path;

    full_path = malloc(ft_strlen(dir) + ft_strlen(cmd) + 2);
    if (!full_path)
        return (NULL);
    ft_strlcpy(full_path, dir, ft_strlen(dir) + 1);
    ft_strlcat(full_path, "/", ft_strlen(dir) + 2);
    ft_strlcat(full_path, cmd, ft_strlen(dir) + ft_strlen(cmd) + 2);
    return (full_path);
}

static char *search_command_in_path(char *path_copy, char *cmd)
{
    char *dir;
    char *full_path;

    dir = ft_strtok(path_copy, ":");
    while (dir)
    {
        full_path = create_full_path(dir, cmd);
        if (!full_path)
            return (NULL);
        if (access(full_path, X_OK) == 0)
            return (full_path);
        free(full_path);
        dir = ft_strtok(NULL, ":");
    }
    return (NULL);
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

static int handle_input(char *input, t_env *env)
{
    (void)env;
    if (!input)
    {
        ft_putstr_fd("\nExiting shell.\n", 1);
        return (1);
    }
    if (*input)
        add_history(input);
    return (0);
}
static int process_command(t_token *tokens, t_env *env, int *exit_status)
{
    t_arg *cmd;
    int result;

    cmd = ft_arg_new(2);
    if (!cmd)
        return (0);
    if (ft_convert_token_to_arg(tokens, cmd, 0) == 0)
    {
        result = execute_command(cmd, env, exit_status);
        free_command(cmd);
        return result;  // Return the result from execute_command
    }
    free_command(cmd);
    return (0);
}

int main_shell_loop(t_env *env)
{
    char *input;
    t_token *tokens;
    int exit_status;

    exit_status = 0;
    sig_init();

    while (1)
    {
        tokens = NULL;
        g_sig.pid = 0;  // Reset pid before each command
        g_sig.sigint = 0;  // Reset sigint flag

        signal(SIGINT, sig_int);
        signal(SIGQUIT, sig_quit);

        input = readline("minishell> ");

        if (input == NULL)
        {
            write(STDERR_FILENO, "exit\n", 6);
            break;
        //     return (exit_status);
        }

        if (g_sig.sigint)//for ctrl-c than echo $?
        {
            //free(input);
            exit_status = g_sig.exit_status;
            g_sig.sigint = 0;  // Reset the flag
           // continue;
        }

        // if (g_sig.sigint)
        // {
        //     // free(input);
        //     continue;  // Skip the rest of the loop if SIGINT was received
        // }


        if (handle_input(input, env))
            return (exit_status);

        int parse_result = parsing(input, &tokens, env, exit_status);
        if (parse_result == 0)
        {
            if (process_command(tokens, env, &exit_status) == -1)
            {
                free_tokens(tokens);
                return (exit_status);
            }
            free_tokens(tokens);
            tokens = NULL;
        }
        else if (parse_result == -1)
        {
            free_tokens(tokens);
            g_sig.exit_status = 1;
            // continue;
        }
        else if (parse_result == 258)
        {
            free_tokens(tokens);
            g_sig.exit_status = 258;
            // continue;
        }

        exit_status = g_sig.exit_status;
    }
    return (exit_status);
}



void free_command(t_arg *cmd)
{
    if (!cmd)
        return;

    // Free arguments
    for (int i = 0; cmd->arg && cmd->arg[i]; i++)
        free(cmd->arg[i]);
    free(cmd->arg);

    // Free redirections
    for (int i = 0; cmd->red && cmd->red[i]; i++)
        free(cmd->red[i]);
    free(cmd->red);

    // Free the command structure itself
    free(cmd);
}

void free_tokens(t_token *tokens)
{
    t_token *tmp;

    while (tokens)
    {
        tmp = tokens;
        tokens = tokens->next;
        free(tmp->content);
        free(tmp);
    }
}

void restore_io(t_io *io)
{
    dup2(io->original_stdin, STDIN_FILENO);
    dup2(io->original_stdout, STDOUT_FILENO);
    close(io->original_stdin);
    close(io->original_stdout);
}

void save_original_io(t_io *io)
{
    io->original_stdin = dup(STDIN_FILENO);
    io->original_stdout = dup(STDOUT_FILENO);
}

int main(int ac, char **av, char **envp)
{
    (void)ac;
    (void)av;
    rl_catch_signals = 0;

    t_io io;
    save_original_io(&io);

    t_env *env = create_env(envp);
    if (!env)
    {
        fprintf(stderr, "Failed to create environment\n");
        return 1;
    }

    int status = main_shell_loop(env);

    //free_env(env);
    restore_io(&io);
    return status;
}