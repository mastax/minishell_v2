/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <elel-bah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 10:51:50 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/07 13:28:02 by elel-bah         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../mini_shell.h"

static int handle_existing_var_append(t_env *env, char *name, char *value, int i)
{
    char *old_value;
    char *new_value;

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
        env->env_vars[i] = ft_concat(name, "=");
        env->env_vars[i] = ft_concat(env->env_vars[i], value);
    }
    return 1;
}

static int handle_existing_var(t_env *env, char *name, char *value, int is_append)
{
    int i;

	i = 0;
    while (env->env_vars[i] && ft_strncmp(env->env_vars[i], name, ft_strlen(name)))
        i++;
    if (!env->env_vars[i])
        return 0;
    if (!value)
        return 1;
    if (is_append)
        return handle_existing_var_append(env, name, value, i);
    free(env->env_vars[i]);
    env->env_vars[i] = ft_concat(name, "=");
    env->env_vars[i] = ft_concat(env->env_vars[i], value);
    return 1;
}

static int find_and_update_var(t_env *env, char *name, char *value, int is_append)
{
    int i;
    size_t name_len;

	i = 0;
	name_len = ft_strlen(name);
    while (i < env->count)
    {
        if (ft_strncmp(env->env_vars[i], name, name_len) == 0 && 
            (env->env_vars[i][name_len] == '=' || env->env_vars[i][name_len] == '\0'))
            return (handle_existing_var(env, name, value, is_append));
        i++;
    }
    return 0;
}

int ft_export(t_env *env, char *s)
{
	char    *name;
	char    *value;
	int     is_append;

	is_append = 0;
	if (check_format(s) == 1)
		return (2);
	parse_export_string(s, &name, &value, &is_append);
	if (!find_and_update_var(env, name, value, is_append))
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

int ft_exports(t_env *env, char **args, int *exit_status)
{
	int i;
	int command_return;

	*exit_status = 0;
	if (!args[1] || (args[1][0] == '\0' && !args[2]))
	{
		print_sorted_env(env);
		return (1);
	}
	i = 1;
	if (args[1][0] == '\0')
		i = 2;
	while (args[i])
    {
		command_return = ft_export(env, args[i]);
		if (command_return == 0)
			return (0);
		else if (command_return == 2)
			get_exit_status(1);
		i++;
	}
	return (1);
}
