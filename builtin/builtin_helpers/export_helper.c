#include "../../mini_shell.h"

int check_format(char *arg)
{
    int cur;
    int has_equal;

    if (!ft_isalpha(arg[0]) && arg[0] != '_')
         return (print_error(1, &arg[0]));
    cur = 0;
    has_equal = 0;
    while (arg[cur])
    {
        if (arg[cur] == '=')
            has_equal = 1;
        else if (arg[cur] == '+' && arg[cur+1] == '=')
            has_equal = 1;
        else if (!ft_isalnum(arg[cur]) && arg[cur] != '_')
            return (print_error(2, &arg[cur]));
        cur++;
    }
    return (0);  // Return 0 (success) regardless of whether there's an equals sign
}

// int check_format(char *arg)
// {
//     int cur;
//     int has_equal;

//     if (!ft_isalpha(arg[0]))
//          return (print_error(1, &arg[0]));
//     cur = 0;
//     has_equal = 0;
//     while (arg[cur])
//     {
//         if (arg[cur] == '=')
//             has_equal = 1;
//         else if (arg[cur] == '+' && arg[cur+1] == '=')
//             has_equal = 1;
//         else if (!ft_isalnum(arg[cur]) && arg[cur] != '_')
//             if (!has_equal)
//                 return (print_error(2, &arg[cur]));
//         cur++;
//     }
//     if (!has_equal)
//         return (1);
//     return (0);
// }

int print_error(int error_code, char *content)
{
    if (error_code == 1) {
        printf("\033[91m%s '%c'\033[0m\n", "export: name cannot start with", *content);
    } else if (error_code == 2) {
        printf("\033[91m%s '%c'\033[0m\n", "export: not valid in this context:", *content);
    }
    return (1);
}

static char **copy_env_vars(t_env *env)
{
    char **sorted_env;
    int i;

    sorted_env = malloc(sizeof(char *) * (env->count + 1));
    if (!sorted_env)
        return (NULL);
    i = 0;
    while (i < env->count)
    {
        sorted_env[i] = ft_strdup(env->env_vars[i]);
        i++;
    }
    sorted_env[i] = NULL;
    return (sorted_env);
}

static void sort_env_vars(char **sorted_env, int count)
{
    int i;
    int j;
    char *temp;

    i = 0;
    while (i < count - 1)
    {
        j = 0;
        while (j < count - i - 1)
        {
            if (ft_strcmp(sorted_env[j], sorted_env[j + 1]) > 0)
            {
                temp = sorted_env[j];
                sorted_env[j] = sorted_env[j + 1];
                sorted_env[j + 1] = temp;
            }
            j++;
        }
        i++;
    }
}

static void print_env_vars(char **sorted_env, int count)
{
    int     i;
    char    *name;
    char    *value;

    i = 0;
    while (i < count)
    {
        ft_putstr_fd("declare -x ", 1);
        name = sorted_env[i];
        value = ft_strchr(sorted_env[i], '=');
        if (value)
        {
            *value = '\0';
            ft_putstr_fd(name, 1);
            ft_putstr_fd("=\"", 1);
            ft_putstr_fd(value + 1, 1);
            ft_putendl_fd("\"", 1);
            *value = '=';
        }
        else
            ft_putendl_fd(sorted_env[i], 1);
        i++;
    }
}

// static void print_env_vars(char **sorted_env, int count)
// {
//     int i;

//     i = 0;
//     while (i < count)
//     {
//         ft_putstr_fd("declare -x ", 1);
//         ft_putendl_fd(sorted_env[i], 1);
//         i++;
//     }
// }

static void free_sorted_env(char **sorted_env, int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        free(sorted_env[i]);
        i++;
    }
    free(sorted_env);
}

void print_sorted_env(t_env *env)
{
    char **sorted_env;

    sorted_env = copy_env_vars(env);
    if (!sorted_env)
        return;
    sort_env_vars(sorted_env, env->count);
    print_env_vars(sorted_env, env->count);
    free_sorted_env(sorted_env, env->count);
}