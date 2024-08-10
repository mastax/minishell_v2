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
                return red[i + 1];
            count++;
        }
        i++;
    }
    return NULL;
}

static void child_process(int pipefd[2], const char *delimiter, t_env *env)
{
    char *line;

    close(pipefd[0]); // Close read end of the pipe
    while ((line = read_line()) != NULL)
    {
        if (strcmp(line, delimiter) == 0)
            break;
        if (expand_variable(&line, env) == -1)
            exit(1);
        write_to_pipe(pipefd, line);
        free(line);
    }
    close(pipefd[1]);
}

static void parent_process(int pipefd[2], pid_t pid)
{
    close(pipefd[1]); // Close write end of the pipe
    waitpid(pid, NULL, 0); // Wait for the child process to finish
}

int create_heredoc(const char *delimiter, t_env *env)
{
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return -1;
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return -1;
    }
    if (pid == 0) // Child process
    {
        child_process(pipefd, delimiter, env);
        exit(0);
    } 
    else // Parent process
    {
        parent_process(pipefd, pid);
        return pipefd[0];
    }
}


//=-=-=-=-=-=-=-=-=--=

// static int create_heredoc(const char *delimiter, t_env *env)
// {
//     char *line;
//     char *expanded_line;
//     size_t len;
//     ssize_t nread;
//     pid_t pid;

//     line = NULL;
//     expanded_line = NULL;
//     len = 0;
//     int pipefd[2];
//     if (pipe(pipefd) == -1)
//     {
//         perror("pipe");
//         return -1;
//     }
//     pid = fork();
//     if (pid == -1)
//     {
//         perror("fork");
//         return -1;
//     }
//     if (pid == 0)
//     { // Child process
//         close(pipefd[0]); // Close read end of the pipe
//         while ((nread = getline(&line, &len, stdin)) != -1)
//         {
//             // Remove newline character if present
//             if (nread > 0 && line[nread - 1] == '\n')
//                 line[nread - 1] = '\0';
            
//             // Check if the line matches the delimiter
//             if (strcmp(line, delimiter) == 0)
//                 break;
            
//             // Expand variables in the line
//             expanded_line = ft_strdup(line);
//             if (expanded_line == NULL)
//             {
//                 perror("ft_strdup");
//                 free(line);
//                 exit(1);
//             }
            
//             if (ft_expand_variable(&expanded_line, env, WORD, 0) == -1) {
//                 free(expanded_line);
//                 free(line);
//                 exit(1);
//             }
            
//             // Write the expanded line to the pipe, including the newline
//             write(pipefd[1], expanded_line, ft_strlen(expanded_line));
//             write(pipefd[1], "\n", 1);
            
//             free(expanded_line);
//             expanded_line = NULL;
//         }
//         free(line);
//         close(pipefd[1]);
//         exit(0);
//     } 
//     else
//     { // Parent process
//         close(pipefd[1]); // Close write end of the pipe
//         waitpid(pid, NULL, 0); // Wait for the child process to finish
//         return pipefd[0];
//     }
// }

// static int create_heredoc(const char *delimiter)
// {
//     char *line;
//     size_t len;
//     ssize_t nread;

//     int pipefd[2];
//     if (pipe(pipefd) == -1) {
//         perror("pipe");
//         return -1;
//     }
//     pid_t pid = fork();
//     if (pid == -1) {
//         perror("fork");
//         return -1;
//     }
//     if (pid == 0)
//     { // Child process
//         close(pipefd[0]); // Close read end of the pipe
//         line = NULL;
//         len = 0;
//         while ((nread = getline(&line, &len, stdin)) != -1)
//         {
//             // Check if the line matches the delimiter
//             if (nread > 0 && line[nread - 1] == '\n')
//                 line[nread - 1] = '\0';
//             if (strcmp(line, delimiter) == 0)
//                 break;
//             // Write the line to the pipe, including the newline
//             line[nread - 1] = '\n'; // Ensure newline character is added back
//             write(pipefd[1], line, nread);
//         }
//         free(line);
//         close(pipefd[1]);
//         exit(0);
//     } 
//     else
//     { // Parent process
//         close(pipefd[1]); // Close write end of the pipe
//         waitpid(pid, NULL, 0); // Wait for the child process to finish
//         return pipefd[0];
//     }
// }

int *handle_heredocs(char **red, int count, t_env *env)
{
    int *heredoc_fds = malloc(sizeof(int) * count);
    if (!heredoc_fds)
        return NULL;

    for (int i = 0; i < count; i++)
    {
        char *delimiter = get_heredoc_delimiter(red, i);
        heredoc_fds[i] = create_heredoc(delimiter, env);
        if (heredoc_fds[i] == -1)
        {
            // Clean up previously created here-docs
            for (int j = 0; j < i; j++)
                close(heredoc_fds[j]);
            free(heredoc_fds);
            return NULL;
        }
    }
    return (heredoc_fds);
}

// int *handle_heredocs(char **red, int count)
// {
//     int *heredoc_fds = malloc(sizeof(int) * count);
//     if (!heredoc_fds)
//         return NULL;

//     for (int i = 0; i < count; i++)
//     {
//         char *delimiter = get_heredoc_delimiter(red, i);
//         heredoc_fds[i] = create_heredoc(delimiter);
//         if (heredoc_fds[i] == -1)
//         {
//             // Clean up previously created here-docs
//             for (int j = 0; j < i; j++)
//                 close(heredoc_fds[j]);
//             free(heredoc_fds);
//             return NULL;
//         }
//     }
//     return (heredoc_fds);
// }