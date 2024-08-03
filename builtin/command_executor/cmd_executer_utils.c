#include "../../mini_shell.h"

// t_env *allocate_env(int count)
// {
//     t_env *env;

//     env = malloc(sizeof(t_env));
//     if (!env)
//         return (NULL);
//     env->env_vars = malloc(sizeof(char*) * (count + 1));
//     if (!env->env_vars)
//     {
//         free(env);
//         return (NULL);
//     }
//     env->count = count;
//     return (env);
// }

// t_env *create_env(char **envp)
// {
//     t_env *env;
//     int count;
//     int i;

//     count = 0;
//     while (envp[count])
//         count++;
//     env = allocate_env(count);
//     if (!env)
//         return (NULL);
//     i = 0;
//     while (i < count && (env->env_vars[i] = ft_strdup(envp[i])))
//         i++;
//     if (i < count)
//     {
//         while (--i >= 0)
//             free(env->env_vars[i]);
//         free(env->env_vars);
//         free(env);
//         return (NULL);
//     }
//     env->env_vars[count] = NULL;
//     return (env);
// }

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

char **split_line(char *line)
{
    int bufsize = MAX_ARGS;
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    token = ft_strtok(line, DELIMITERS);
    while (token != NULL)
    {
        tokens[position] = ft_strdup(token);
        position++;

        if (position >= bufsize)
        {
            bufsize += MAX_ARGS;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens)
            {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }

        token = ft_strtok(NULL, DELIMITERS);
    }
    tokens[position] = NULL;
    return tokens;
}

int is_builtin(const char *cmd)
{
    const char *builtins[] = {
        "echo", "cd", "pwd", "export", "unset", "env", "exit", NULL
    };

    for (int i = 0; builtins[i]; i++)
    {
        if (strcmp(cmd, builtins[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}