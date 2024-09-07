/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <elel-bah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 10:51:06 by elel-bah          #+#    #+#             */
/*   Updated: 2024/08/29 10:51:07 by elel-bah         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../mini_shell.h"

int pwd(int fd)
{
    char current_wd[PATH_MAX];

    if (getcwd(current_wd, sizeof(current_wd)) == NULL)
    {
        ft_putendl_fd(current_wd, STDOUT_FILENO);
        return (1);
    }
    ft_putendl_fd(current_wd, fd);
    return (0);
}
