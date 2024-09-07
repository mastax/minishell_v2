/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <elel-bah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 20:59:26 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/07 14:37:51 by elel-bah         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../mini_shell.h"

static char *get_heredoc_delimiter(char **red, int index)
{
    int count = 0;
    int i = 0;
    while (red && red[i])
    {
        if (ft_strcmp(red[i], "<<") == 0)
        {
            if (count == index && red[i + 1])
                return ft_strdup(red[i + 1]);
            count++;
        }
        i++;
    }
    return NULL;
}

int process_delimiter(char **red, int i, char **delimiter, char **processed_delimiter, t_fd_tracker *tracker)
{
    int j;

    j = 0;
    *delimiter = get_heredoc_delimiter(red, i);
    if (!*delimiter)
        return 0;
    while (delimiter[0][j])
    {
        if (delimiter[0][j] == '$' && (delimiter[0][j + 1] == '"'
			|| delimiter[0][j + 1] == '\''))
		{
            delimiter[0] = ft_remove_char(delimiter[0], j);
            if (!delimiter[0])
                return (0);
        }
        j++;
    }
    tracker->qout = check_if_qoutes(*delimiter);
    if (tracker->qout == 1)
        *processed_delimiter = ft_remove_quotes(*delimiter);
    else
        *processed_delimiter = *delimiter;
    return *processed_delimiter != NULL;
}
int create_heredoc(const char *delimiter, t_env *env, t_fd_tracker *tracker)
{
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return -1;
    }
    track_fd(tracker, pipefd[0]);
    track_fd(tracker, pipefd[1]);
    get_in_heredoc(1);/////
    pid = fork();
    if (pid == -1) {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        untrack_fd(tracker, pipefd[0]);
        untrack_fd(tracker, pipefd[1]);
        get_in_heredoc(0);  // Reset the flag
        return -1;
    }
    if (pid == 0) { // Child process
        signal(SIGINT, SIG_DFL);
        child_process(pipefd, delimiter, env, tracker);
        exit(0);
    }
    else
    { // Parent process
        parent_process(pipefd, pid, tracker);
        get_in_heredoc(0);  // Reset the flag
        untrack_fd(tracker, pipefd[1]); // Untrack the write end that we closed
        return pipefd[0];
    }
} 

void cleanup_and_return(int *heredoc_fds, int count, t_fd_tracker *tracker)
{
    int j;

    j = 0;
    while (j < count)
    {
        close(heredoc_fds[j]);
        untrack_fd(tracker, heredoc_fds[j]);
        j++;
    }
    free(heredoc_fds);
}
int *handle_heredocs(char **red, int count, t_env *env, t_fd_tracker *tracker)
{
    int     *heredoc_fds;
    char    *delimiter;
    char    *processed_delimiter;
    int     i;

    heredoc_fds = malloc(sizeof(int) * count);
    if (!heredoc_fds)
        return NULL;
    i = 0;
    while (i < count)
    {
        if (!process_delimiter(red, i, &delimiter, &processed_delimiter, tracker))
            return(cleanup_and_return(heredoc_fds, i, tracker), NULL);
        heredoc_fds[i] = create_heredoc(processed_delimiter, env, tracker);
        free(processed_delimiter);
        if (heredoc_fds[i] == -1)
            return (cleanup_and_return(heredoc_fds, i, tracker), NULL);
        i++;
    }
    return heredoc_fds;
}
