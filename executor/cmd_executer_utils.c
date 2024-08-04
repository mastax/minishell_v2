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

// char **split_line(char *line)
// {
//     int bufsize = MAX_ARGS;
//     int position = 0;
//     char **tokens = malloc(bufsize * sizeof(char*));
//     char *token;

//     if (!tokens)
//     {
//         perror("malloc");
//         exit(EXIT_FAILURE);
//     }

//     token = ft_strtok(line, DELIMITERS);
//     while (token != NULL)
//     {
//         tokens[position] = ft_strdup(token);
//         position++;

//         if (position >= bufsize)
//         {
//             bufsize += MAX_ARGS;
//             tokens = realloc(tokens, bufsize * sizeof(char*));
//             if (!tokens)
//             {
//                 perror("realloc");
//                 exit(EXIT_FAILURE);
//             }
//         }

//         token = ft_strtok(NULL, DELIMITERS);
//     }
//     tokens[position] = NULL;
//     return tokens;
// }

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