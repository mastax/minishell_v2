#include "../mini_shell.h"

static int handle_output_redirection(const char *filename, int flags)
{
    int fd = open(filename, flags, 0644);
    if (fd == -1)
    {
        perror("open");
        return -1;
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

static int handle_input_redirection(const char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return -1;
    }
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

static int process_redirection(char **red, int *i)
{
    const char *error_msg;

    if (!red[*i + 1])
    {
        error_msg = "Syntax error: missing filename after redirection\n";
        write(STDERR_FILENO, error_msg, strlen(error_msg));
        return -1;
    }
    if (ft_strcmp(red[*i], ">") == 0)
        return handle_output_redirection(red[*i + 1], O_WRONLY | O_CREAT | O_TRUNC);
    else if (ft_strcmp(red[*i], ">>") == 0)
        return handle_output_redirection(red[*i + 1], O_WRONLY | O_CREAT | O_APPEND);
    else if (ft_strcmp(red[*i], "<") == 0)
        return handle_input_redirection(red[*i + 1]);
    return -1;
}

int apply_redirections(char **red)
{
    int i = 0;

    if (!red || !red[0])
        return 0;
    while (red[i])
    {
        if (ft_strcmp(red[i], ">") == 0 || ft_strcmp(red[i], ">>") == 0 || 
            ft_strcmp(red[i], "<") == 0)
        {
            if (process_redirection(red, &i) == -1)
                return -1;
            i += 2;
        }
        else
            i++;
    }
    return 0;
}

// int apply_redirections(char **red)
// {
//     int         i;
//     int         fd;
//     const char  *error_msg;

//     i = 0;

//     if (!red || !red[0])
//         return 0;
//     while (red[i])
//     {
//         if (strcmp(red[i], ">") == 0)
//         {
//             if (!red[i+1])
//             {
//                 error_msg = "Syntax error: missing filename after >\n";
//                 write(STDERR_FILENO, error_msg, strlen(error_msg));
//                 return -1;
//             }
//             fd = open(red[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
//             if (fd == -1)
//             {
//                 perror("open");
//                 return -1;
//             }
//             if (dup2(fd, STDOUT_FILENO) == -1)
//             {
//                 perror("dup2");
//                 close(fd);
//                 return -1;
//             }
//             close(fd);
//         }
//         else if (strcmp(red[i], ">") == 0) // Output redirection
//         {
//             fd = open(red[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
//             if (fd == -1)
//             {
//                 perror("open");
//                 return -1;
//             }
//             dup2(fd, STDOUT_FILENO);
//             close(fd);
//         }
//         else if (strcmp(red[i], ">>") == 0) // Append redirection
//         {
//             fd = open(red[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
//             if (fd == -1)
//             {
//                 perror("open");
//                 return -1;
//             }
//             dup2(fd, STDOUT_FILENO);
//             close(fd);
//         }
//         i += 2; // Move to the next redirection
//     }
//     return 0;
// }
