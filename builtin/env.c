#include "../mini_shell.h"

int ft_env(t_env *env)
{
    int i;

    i = 0;
    while (i < env->count)
    {
        if (ft_strchr(env->env_vars[i], '=') != NULL)
        {
            ft_putstr_fd(env->env_vars[i], STDOUT_FILENO);
            write(STDOUT_FILENO, "\n", 1);
        }
        i++;
    }
    return 0;
}
