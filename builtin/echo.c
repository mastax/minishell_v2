/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <elel-bah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 15:51:21 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/01 10:35:07 by elel-bah         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../mini_shell.h"

static int is_valid_n_flag(const char *arg)
{
    int i;
    int is_valid;

    is_valid = 0;
    if (arg[0] == '-')
    {
        i = 1;
        while (arg[i] == 'n')
            i++;
        if (arg[i] == '\0')
            is_valid = 1;
    }
    return (is_valid);
}

int ft_echo(char **av)
{
    int i;
    int flag_n;

    i = 1;
    flag_n = 0;
    while (av[i] != NULL && is_valid_n_flag(av[i]))
    {
        flag_n = 1;
        i++;
    }
    while (av[i] != NULL)
    {
        ft_putstr_fd(av[i], STDOUT_FILENO);
        if (av[i + 1] != NULL)
            write(STDOUT_FILENO, " ", 1);
        i++;
    }
    if (flag_n == 0)
        write(1, "\n", 1);
    return (0);
}
