/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_executer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <elel-bah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:55:02 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/01 10:27:09 by elel-bah         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../mini_shell.h"

int execute_builtin_p(t_arg *cmd, t_env *env, int *exit_status)
{
    if (cmd->arg == NULL || cmd->arg[0] == NULL)
        return (0);
    if (ft_strcmp(cmd->arg[0], "cd") == 0)
        return ft_change_dir(cmd->arg, env, exit_status);
    else if (ft_strcmp(cmd->arg[0], "exit") == 0)
        return ft_exit(cmd->arg, exit_status);
    else if (ft_strcmp(cmd->arg[0], "export") == 0)
        return ft_exports(env, cmd->arg, exit_status);
    else if (ft_strcmp(cmd->arg[0], "unset") == 0)
        return ft_unsets(env, cmd->arg, exit_status);
    return (1); // Command not found
}

int execute_builtin_ch(t_arg *cmd, t_env *env, int *exit_status)
{
    if (cmd->arg == NULL || cmd->arg[0] == NULL)
        return (0);
    if (ft_strcmp(cmd->arg[0], "cd") == 0)
        return ft_change_dir(cmd->arg, env, exit_status);
    else if (ft_strcmp(cmd->arg[0], "echo") == 0)
        return ft_echo(cmd->arg);
    else if (ft_strcmp(cmd->arg[0], "env") == 0)
        return ft_env(env);
    else if (ft_strcmp(cmd->arg[0], "exit") == 0)
        return ft_exit(cmd->arg, exit_status);
    else if (ft_strcmp(cmd->arg[0], "export") == 0)
        return ft_exports(env, cmd->arg, exit_status);
    else if (ft_strcmp(cmd->arg[0], "pwd") == 0)
        return pwd(STDOUT_FILENO);
    else if (ft_strcmp(cmd->arg[0], "unset") == 0)
        return ft_unsets(env, cmd->arg, exit_status);
    return (1); // Command not found
}
