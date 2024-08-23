#include "../../mini_shell.h"

int check_format(char *arg)
{
    int cur;
    int has_equal;

    if (!ft_isalpha(arg[0]) && arg[0] != '_')
        return (print_error(1, arg), 1);
    cur = 0;
    while (arg[cur] != '\0')
    {
        if (arg[cur] == '=')
            break ;
        if (arg[cur] == '-')
            return (print_error(1, arg), 1);
        cur++;
    }
    cur = 0;
    has_equal = 0;
    while (arg[cur])
    {
        if (arg[cur] == '=')
            has_equal = 1;
        else if (arg[cur] == '+' && arg[cur+1] == '=')
            has_equal = 1;
        
        cur++;
    }
    return (0);  // Return 0 (success) regardless of whether there's an equals sign
}

int print_error(int error_code, char *content)
{
    if (error_code == 1) {
        ft_putstr_fd("export: `", 2);
        ft_putstr_fd(content, 2);
        ft_putstr_fd("': not a valid identifier\n", 2);
    }
    return (1);
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