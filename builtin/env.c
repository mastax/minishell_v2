/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <elel-bah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:55:12 by elel-bah          #+#    #+#             */
/*   Updated: 2024/08/29 16:55:13 by elel-bah         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../mini_shell.h"

int ft_env(t_env *env)
{
    int i;

    i = 0;
    while (i < env->count)
    {
        if (ft_strchr(env->env_vars[i], '=') != NULL)
        {
            ft_putstr_fd(env->env_vars[i], STDOUT_FILENO);
            write(STDOUT_FILENO, "\n", 1);
        }
        i++;
    }
    return 0;
}
