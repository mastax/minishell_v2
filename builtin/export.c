#include "../mini_shell.h"

static int handle_existing_var(t_env *env, char *name, char *value, int is_append, char *s, int i)
{
    char *old_value, *new_value;
    
    if (value == NULL) // If no value provided, don't change the existing value
        return (1);
    
    if (is_append)
    {
        old_value = ft_strchr(env->env_vars[i], '=');
        if (old_value)
        {
            old_value++;
            new_value = ft_concat(old_value, value);
            free(env->env_vars[i]);
            env->env_vars[i] = ft_concat(name, "=");
            env->env_vars[i] = ft_concat(env->env_vars[i], new_value);
            free(new_value);
        }
        else
        {
            free(env->env_vars[i]);
            env->env_vars[i] = ft_concat(s, value);
        }
    }
    else
    {
        free(env->env_vars[i]);
        env->env_vars[i] = ft_strdup(s);
    }
    return (1);
}

// static int handle_existing_var(t_env *env, char *name, char *value, int is_append, char *s, int i)
// {
//     char *old_value, *new_value;
    
//     if (is_append)
//     {
//         old_value = ft_strchr(env->env_vars[i], '=') + 1;
//         new_value = ft_strjoin(old_value, value);
//         free(env->env_vars[i]);
//         env->env_vars[i] = ft_strjoin(name, "=");
//         env->env_vars[i] = ft_strjoin(env->env_vars[i], new_value);
//         free(new_value);
//     }
//     else
//     {
//         free(env->env_vars[i]);
//         env->env_vars[i] = ft_strdup(s);
//     }
//     return (1);
// }

static int find_and_update_var(t_env *env, char *name, char *value, int is_append, char *s)
{
    for (int i = 0; i < env->count; i++)
    {
        if (ft_strncmp(env->env_vars[i], name, ft_strlen(name)) == 0 && 
            (env->env_vars[i][ft_strlen(name)] == '=' || env->env_vars[i][ft_strlen(name)] == '\0'))
        {
            return (handle_existing_var(env, name, value, is_append, s, i));
        }
    }
    return (0);
}

static int parse_export_string(char *s, char **name, char **value, int *is_append)
{
    *name = ft_strdup(s);
    *value = ft_strchr(*name, '=');
    if (*value)
    {
        **value = '\0';
        (*value)++;
        if (*(*value - 2) == '+')
        {
            *is_append = 1;
            *(*value - 2) = '\0';
        }
    }
    else
        *value = NULL;
    return (0);
}


int append_new_var(t_env *env, char *s)
{
    char **new_env;
    int i;

    new_env = malloc(sizeof(char*) * (env->count + 2));
    if (!new_env)
        return (0);
    i = 0;
    while (i < env->count)
    {
        new_env[i] = env->env_vars[i];
        i++;
    }
    new_env[env->count] = ft_strdup(s);
    if (!new_env[env->count])
    {
        free(new_env);
        return (0);
    }
    new_env[env->count + 1] = NULL;
    free(env->env_vars);
    env->env_vars = new_env;
    env->count++;
    return (1);
}

int ft_export(t_env *env, char *s)
{
    char    *name;
    char    *value;
    int     is_append;

    is_append = 0;
    if (check_format(s))
        return (2);
    parse_export_string(s, &name, &value, &is_append);
    
    if (!find_and_update_var(env, name, value, is_append, s))
    {
        if (!append_new_var(env, s))
        {
            free(name);
            return (0);
        }
    }
    
    free(name);
    return (1);
}

// int ft_export(t_env *env, char *s)
// {
//     char    *name;
//     char    *value;
//     int     is_append;

//     is_append = 0;
//     if (check_format(s))
//         return (2);
//     parse_export_string(s, &name, &value, &is_append);
    
//     if (!value)  // No '=' in the string
//     {
//         if (!find_and_update_var(env, name, NULL, 0, s))
//         {
//             if (!append_new_var(env, name))  // Pass only the name
//             {
//                 free(name);
//                 return (0);
//             }
//         }
//     }
//     else
//     {
//         if (!find_and_update_var(env, name, value, is_append, s))
//         {
//             if (!append_new_var(env, s))
//             {
//                 free(name);
//                 return (0);
//             }
//         }
//     }
    
//     free(name);
//     return (1);
// }

// int ft_export(t_env *env, char *s)
// {
//     char    *name;
//     char    *value;
//     int     is_append;

//     is_append = 0;
//     if (check_format(s))
//         return (2);
//     parse_export_string(s, &name, &value, &is_append);
//     if (find_and_update_var(env, name, value, is_append, s))
//     {
//         free(name);
//         return (1);
//     }
//     if (!append_new_var(env, s))
//     {
//         free(name);
//         return (0);
//     }
//     free(name);
//     return (1);
// }

int ft_exports(t_env *env, char **args, int *exit_status)
{
    int i;
    int command_return;

    *exit_status = 0;
    if (!args[1])
    {
        print_sorted_env(env);
        return (1);
    }

    i = 1;
    while (args[i]) {
        command_return = ft_export(env, args[i]);
        if (command_return == 0)
            return (0);
        else if (command_return == 2)
            *exit_status = 1;
        i++;
    }
    return (1);
}

// int ft_exports(t_env *env, char **args, int *exit_status)
// {
//     int i;
//     int command_return;

//     i = 1;
//     *exit_status = 0;
//     while (args[i]) {
//         command_return = ft_export(env, args[i]);
//         if (command_return == 0)
//             return (0);
//         else if (command_return == 2)
//             *exit_status = 1;
//         i++;
//     }
//     return (1);
// }

