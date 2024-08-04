#include "../mini_shell.h"

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
