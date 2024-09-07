/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <elel-bah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 17:03:53 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/07 16:17:57 by elel-bah         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../mini_shell.h"

void sig_int(int code)
{
    (void)code;
    if (get_in_heredoc(-500))
    {
        write(STDERR_FILENO, "\n", 1);
        get_sigint(1);
        get_exit_status(1);
    }
    else if (get_pid(-500) == 0)
    {
        write(STDERR_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
        get_sigint(1);
        get_exit_status(1);
    }
    else
        write(STDERR_FILENO, "\n", 1);
}

void sig_quit(int code)
{
    char *nbr;
    nbr = ft_itoa(code);
    if (get_pid(-500) != 0)
    {
        ft_putstr_fd("Quit: \n", STDERR_FILENO);
        ft_putendl_fd(nbr, STDERR_FILENO);
       	get_exit_status(131);
        get_sigquit(1);
    }
    // else
    //     ft_putstr_fd("\b\b  \b\b", STDERR_FILENO);
    free(nbr);
}

void sig_init(void)
{
    get_sigint(0);
    get_sigquit(0);
    get_pid(0);
    get_exit_status(0);
}

int get_sigint(int n)
{
	static int v = 0;
	
	if (n != -500)
		v = n;
	return (v);
}
int get_sigquit(int n)
{
	static int v = 0;
	
	if (n != -500)
		v = n;
	return (v);
}
int get_pid(int n)
{
	static int v = 0;
	
	if (n != -500)
		v = n;
	return (v);
}
int get_exit_status(int n)
{
	static int v = 0;
	
	if (n != -500)
		v = n;
	return (v);
}
int get_in_heredoc(int n)
{
	static int v = 0;
	
	if (n != -500)
		v = n;
	return (v);
}

