/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <elel-bah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:54:52 by elel-bah          #+#    #+#             */
/*   Updated: 2024/08/30 16:03:37 by elel-bah         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../mini_shell.h"

int check_format(char *arg)
{
    int cur;

    if (!ft_isalpha(arg[0]) && arg[0] != '_')
        return (print_error(1, arg), 1);
    
    cur = 0;
    while (arg[cur] != '\0')
    {
        if (arg[cur] == '=')
            break;
        if (arg[cur] == '-')
            return (print_error(1, arg), 1);
        cur++;
    }
    return 0;  // valide format without '='
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
int parse_export_string(char *s, char **name, char **value, int *is_append)
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