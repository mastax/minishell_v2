#include "../../mini_shell.h"

static char *create_new_var(const char *key, const char *value)
{
    size_t  key_len;
    size_t  value_len;
    char    *new_var;

    key_len = ft_strlen(key);
    value_len = ft_strlen(value);
    new_var = malloc(key_len + value_len + 2);
    if (!new_var)
        return (NULL);
    ft_strcpy(new_var, key);
    new_var[key_len] = '=';
    ft_strcpy(new_var + key_len + 1, value);
    return (new_var);
}

static int update_existing_env(t_env *env, const char *key, char *new_var)
{
    int i;
    size_t key_len;

    key_len = ft_strlen(key);
    i = 0;
    while (i < env->count)
    {
        if (ft_strncmp(env->env_vars[i], key, key_len) == 0 && 
            env->env_vars[i][key_len] == '=')
        {
            free(env->env_vars[i]);
            env->env_vars[i] = new_var;
            return 1;
        }
        i++;
    }
    return 0;
}

static int add_new_env(t_env *env, char *new_var)
{
    int i;
    char **new_env;

    new_env = malloc(sizeof(char*) * (env->count + 2));
    if (!new_env)
    {
        free(new_var);
        return (0);
    }
    i = 0;
    while (i < env->count)
    {
        new_env[i] = env->env_vars[i];
        i++;
    }
    new_env[env->count] = new_var;
    new_env[env->count + 1] = NULL;
    free(env->env_vars);
    env->env_vars = new_env;
    env->count++;
    return 1;
}

int set_env_value(t_env *env, const char *key, const char *value)
{
    char *new_var;

    new_var = create_new_var(key, value);
    if (!new_var)
        return 0;
    if (update_existing_env(env, key, new_var))
        return 1;
    return (add_new_env(env, new_var));
}

