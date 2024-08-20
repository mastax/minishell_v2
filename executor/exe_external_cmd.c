#include "../mini_shell.h"

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
    {
        status = handle_parent_process(pid);
        if (cmd_path != argv[0])
            free(cmd_path);
    }
    g_sig.exit_status = status;
    return (status);
}