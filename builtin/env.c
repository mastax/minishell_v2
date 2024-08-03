#include "../mini_shell.h"

// t_env *create_env(char **envp)
// {
//     t_env *env = malloc(sizeof(t_env));
//     if (!env) return NULL;

//     int count = 0;
//     while (envp[count]) count++;
//     env->env_vars = malloc(sizeof(char*) * (count + 1));
//     if (!env->env_vars)
//     {
//         free(env);
//         return NULL;
//     }
//     for (int i = 0; i < count; i++)
//     {
//         env->env_vars[i] = ft_strdup(envp[i]);
//         if (!env->env_vars[i])
//         {
//             for (int j = 0; j < i; j++)
//                 free(env->env_vars[j]);
//             free(env->env_vars);
//             free(env);
//             return NULL;
//         }
//     }
//     env->env_vars[count] = NULL;
//     env->count = count;
//     return env;
// }

// void free_env(t_env *env)
// {
//     int i;

//     i = 0;
//     if (!env)
//         return;
//     while (i < env->count)
//     {
//         free(env->env_vars[i]);
//         i++;
//     }
//     free(env->env_vars);
//     free(env);
// }

// char *get_env_value(t_env *env, const char *key)
// {
//     int key_len;
//     int i;

//     key_len = strlen(key);
//     i = 0;
//     while (i < env->count)
//     {
//         if (strncmp(env->env_vars[i], key, key_len) == 0 && env->env_vars[i][key_len] == '=')
//             return env->env_vars[i] + key_len + 1;
//         i++;
//     }
//     return NULL;
// }

int ft_env(t_env *env)
{
    int i;

    i = 0;
    while (i < env->count)
    {
        ft_putstr_fd(env->env_vars[i], STDOUT_FILENO);
        write(STDOUT_FILENO, "\n", 1);
        i++;
    }
    return 0;
}
