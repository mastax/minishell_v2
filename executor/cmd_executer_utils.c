/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_executer_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <elel-bah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 21:17:23 by elel-bah          #+#    #+#             */
/*   Updated: 2024/08/29 11:01:47 by elel-bah         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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

int is_builtin(const char *cmd)
{
    const char *builtins[] = {
        "echo", "cd", "pwd", "export", "unset", "env", "exit", NULL
    };
    if (cmd == NULL)
        return (0);
    for (int i = 0; builtins[i]; i++)
    {
        if (ft_strcmp(cmd, builtins[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}
