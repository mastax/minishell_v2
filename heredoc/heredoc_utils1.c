/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <elel-bah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 21:16:39 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/07 14:51:52 by elel-bah         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../mini_shell.h"

void child_process(int pipefd[2], const char *delimiter, t_env *env, t_fd_tracker *fd_tracker)
{
    char *line;

    close(pipefd[0]); // Close read end of the pipe
    untrack_fd(fd_tracker, pipefd[0]);
    
    while ((line = read_line()) != NULL)
    {        if (ft_strcmp(line, delimiter) == 0)
            break;
        if (fd_tracker->qout == 0)
            if (expand_variable(&line, env) == -1)
                exit(1);
        write_to_pipe(pipefd, line);
        free(line);
    }
    close(pipefd[1]);
    untrack_fd(fd_tracker, pipefd[1]);
}

void parent_process(int pipefd[2], pid_t pid, t_fd_tracker *fd_tracker) {
    (void)pid;
    int status;
    close(pipefd[1]); // Close write end of the pipe
    untrack_fd(fd_tracker, pipefd[1]);
    waitpid(pid, &status, 0);
}
int check_if_qoutes(char *s)
{
    int i;

    i = 0;
    while (s[i])
    {
        if (s[i] == '"' || s[i] == '\'')
            return (1);
        i++;
    }
    return (0);
}