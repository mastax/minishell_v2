/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <elel-bah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 10:51:31 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/01 12:22:04 by elel-bah         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../mini_shell.h"

int unset_env_value(t_env *env, const char *key)
{
    int i;
    int j;
    size_t key_len;

    i = 0;
    key_len = ft_strlen(key);
    while (i < env->count)
    {
        if (ft_strncmp(env->env_vars[i], key, key_len) == 0 && 
            (env->env_vars[i][key_len] == '=' || env->env_vars[i][key_len] == '\0'))
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
    int track_err;

    i = 1;
    track_err = 0;
    while (args[i])
    {
        if (check_format(args[i]) == 1)
            track_err = 1;
        else
            unset_env_value(env, args[i]);
        i++;
    }
    *exit_status = track_err;
    return (1);
}

