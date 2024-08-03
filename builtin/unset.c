#include "../mini_shell.h"

int unset_env_value(t_env *env, const char *key)
{
    int i;
    int j;
    size_t key_len;

    i = 0;
    j = 0;
    key_len = ft_strlen(key);
    while (i < env->count)
    {
        if (ft_strncmp(env->env_vars[i], key, key_len) == 0 && env->env_vars[i][key_len] == '=')
        {
            free(env->env_vars[i]);
            j = i;
            while (j < env->count - 1)
            {
                env->env_vars[j] = env->env_vars[j + 1];
                j++;
            }
            env->count--;
            env->env_vars[env->count] = NULL;
            return 1;
        }
        i++;
    }
    return 0;
}

int ft_unsets(t_env *env, char **args, int *exit_status)
{
    int i;

    i = 0;
    *exit_status = 0;
    while (args[++i])
    {
        if (!unset_env_value(env, args[i]))
        {
            (*exit_status)++;
        }
    }
    return 1;
}
