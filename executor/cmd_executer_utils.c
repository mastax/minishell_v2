#include "../mini_shell.h"

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
