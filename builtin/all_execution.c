#ifndef STRUCT_H
#define STRUCT_H

typedef struct x_arg
{
    char  **arg;
    char  **red;
    struct x_arg *next;
} t_arg;

typedef enum s_type
{
	WORD,
	PIPE,
	RED_IN,
	RED_OUT,
	HER_DOC,
	APPEND,
}	t_type;

typedef struct s_tokenz
{
	char			*content;
	int				type;
	struct s_tokenz	*next;
}	t_token;

// Add this to your header file (mini_shell.h)
typedef struct s_env
{
    char    **env_vars;
    int     count;
} t_env;

typedef struct s_io
{
    int original_stdin;
    int original_stdout;
} t_io;

typedef struct s_redir
{
    int fd;
    int original_fd;
    int is_output;
} t_redir;

#endif

void	ft_putstr_fd(char *str, int fd)
{
	if (!str)
		write (fd, "(null)\n", 7);
	else
		write (fd, str, ft_strlen(str));
}

int is_valid_var(char c)
{
	if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z')
	|| (c >= 'A' && c <= 'Z') || c == '_')
		return (1);
	else
		return (0);
}

char	*ft_strdup(char *s1)
{
	char	*s;
	int		i;

	i = 0;
	s = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (s == NULL)
		return (NULL);
	while (s1[i])
	{
		s[i] = s1[i];
		i++;
	}
	s[i] = '\0';
	return (s);
}

char	*ft_strndup(char *str, int n)
{
	char	*res;
	int		i;

	i = 0;
	res = malloc(sizeof(char) * (n + 1));
	if (!res)
		return (NULL);
	while (str[i] != '\0' && i < n)
	{
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char	*ft_strjoin(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*s;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1)
		return (ft_strdup(s2));
	else if (!s2)
		return (ft_strdup(s1));
	s = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!s)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		s[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
		s[i++] = s2[j++];
	s[i] = '\0';
	free(s1);
	return (s);
}

//pip

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
    char **argv = split_line(command);
    int status = 0;
    int pid = 0;

    if (is_builtin(argv[0]))
    {
        int temp_stdin = dup(STDIN_FILENO);
        int temp_stdout = dup(STDOUT_FILENO);

        redirect_io(input_fd, output_fd);
        t_arg temp_cmd;
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

int handle_pipeline(char **commands, t_env *env)
{
    int pipe_fds[2];
    int prev_input = STDIN_FILENO;
    int temp_stdout = dup(STDOUT_FILENO);
    int pid, last_pid = 0;
    int status = 0;

    while (*commands)
    {
        if (*(commands + 1))
            pipe(pipe_fds);
        else
            pipe_fds[1] = temp_stdout;
        pid = run_pipeline_command(*commands, env, prev_input, pipe_fds[1]);
        if (prev_input != STDIN_FILENO)
            close(prev_input);
        if (pipe_fds[1] != temp_stdout)
            close(pipe_fds[1]);
        prev_input = pipe_fds[0];
        last_pid = pid;
        commands++;
    }
    waitpid(last_pid, &status, 0);
    if (prev_input != STDIN_FILENO)
        close(prev_input);
    close(temp_stdout);
    return WEXITSTATUS(status);
}

//builtin

int execute_builtin(t_arg *cmd, t_env *env, int *exit_status)
{
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
//redirections

int apply_redirections(char **red)
{
    int i = 0;
    int fd;

    if (!red || !red[0])
    {
        return 0;
    }

    while (red[i])
    {
        if (strcmp(red[i], ">") == 0)
        {
            if (!red[i+1])
            {
                fprintf(stderr, "Syntax error: missing filename after >\n");
                return -1;
            }
            fd = open(red[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
            {
                perror("open");
                return -1;
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                return -1;
            }
            close(fd);
        }
        else if (strcmp(red[i], ">") == 0) // Output redirection
        {
            fd = open(red[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
            {
                perror("open");
                return -1;
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (strcmp(red[i], ">>") == 0) // Append redirection
        {
            fd = open(red[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1)
            {
                perror("open");
                return -1;
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        i += 2; // Move to the next redirection
    }
    return 0;
}

static char *find_command(char *cmd, char **envp)
{
    char *path;
    char *path_copy;
    char *dir;
    char *full_path;

    path = NULL;
    path_copy = NULL;
    full_path = NULL;
    // Find the PATH variable
    for (int i = 0; envp[i]; i++)
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
        {
            path = envp[i] + 5;
            break;
        }
    }
    if (!path)
        return NULL;

    path_copy = ft_strdup(path);
    if (!path_copy)
        return NULL;

    dir = ft_strtok(path_copy, ":");
    while (dir)
    {
        full_path = malloc(ft_strlen(dir) + ft_strlen(cmd) + 2);
        if (!full_path)
        {
            free(path_copy);
            return NULL;
        }
        snprintf(full_path, ft_strlen(dir) + ft_strlen(cmd) + 2, "%s/%s", dir, cmd);
        // snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
        // sprintf(full_path, "%s/%s", dir, cmd);
        
        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return full_path;
        }
        
        free(full_path);
        dir = ft_strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

int execute_external_command(char **argv, char **envp)
{
    pid_t pid;
    int status;
    char *cmd_path;

    if (access(argv[0], X_OK) == 0)
        cmd_path = argv[0];
    else
    {
        cmd_path = find_command(argv[0], envp);
        if (!cmd_path)
        {
            fprintf(stderr, "%s: command not found\n", argv[0]);/////change
            return (127);  // Command not found
        }
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return 1;
    }
    else if (pid == 0)
    {
        // Child process
        if (execve(cmd_path, argv, envp) == -1)
        {
            perror("execve");
            exit(1);
        }
    }
    else
    {
        // Parent process
        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid");
            return 1;
        }
        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            return 128 + WTERMSIG(status);
    }
    if (cmd_path != argv[0])
        free(cmd_path);
    return 0;
}

void save_original_io(t_io *io)
{
    io->original_stdin = dup(STDIN_FILENO);
    io->original_stdout = dup(STDOUT_FILENO);
}

void restore_io(t_io *io)
{
    dup2(io->original_stdin, STDIN_FILENO);
    dup2(io->original_stdout, STDOUT_FILENO);
    close(io->original_stdin);
    close(io->original_stdout);
}

//this main loop need to adabt to the part of parsinf of my friend

int main_shell_loop(t_env *env)
{
    char *input;
    char **pipeline_commands;
    t_arg *cmd;
    int exit_status = 0;

    while (1)
    {
        // Display prompt and read input
        input = readline("minishell> ");
        // Check for EOF (Ctrl+D)
        if (!input)
        {
            printf("\nExiting shell.\n");
            return exit_status;
        }
        // Add input to history
        if (*input)
            add_history(input);
        // Check for pipeline
        pipeline_commands = split_pipeline(input);
        if (pipeline_commands[1] != NULL) // We have a pipeline
        {
            exit_status = handle_pipeline(pipeline_commands, env);
        }
        else // Single command
        {
            // Parse the input
            cmd = parse_command(pipeline_commands[0]); //need to switch with the parsing of my friend
            if (cmd)
            {
                // Execute the command
                exit_status = execute_command(cmd, env, &exit_status);
                // Free the command structure
                free_command(cmd);
            }
        }
        // Free the pipeline commands
        free_argv(pipeline_commands);
        // Free the input string
        free(input);
        // Check if we need to exit the shell
        if (exit_status == -1)
        {
            printf("Exiting shell.\n");
            return 0;
        }
    }
    return exit_status;
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