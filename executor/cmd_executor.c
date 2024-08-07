#include "../mini_shell.h"

static char *find_command(char *cmd, char **envp)
{
    char *path;
    char *path_copy;
    char *dir;
    char *full_path;

    path = NULL;
    path_copy = NULL;
    full_path = NULL;
    // Find the PATH variable
    for (int i = 0; envp[i]; i++)
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
        {
            path = envp[i] + 5;
            break;
        }
    }
    if (!path)
        return NULL;

    path_copy = ft_strdup(path);
    if (!path_copy)
        return NULL;

    dir = ft_strtok(path_copy, ":");
    while (dir)
    {
        full_path = malloc(ft_strlen(dir) + ft_strlen(cmd) + 2);
        if (!full_path)
        {
            free(path_copy);
            return NULL;
        }
        snprintf(full_path, ft_strlen(dir) + ft_strlen(cmd) + 2, "%s/%s", dir, cmd);
        // snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
        // sprintf(full_path, "%s/%s", dir, cmd);
        
        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return full_path;
        }
        
        free(full_path);
        dir = ft_strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

int execute_external_command(char **argv, char **envp)
{
    pid_t pid;
    int status;
    char *cmd_path;

    if (access(argv[0], X_OK) == 0)
        cmd_path = argv[0];
    else
    {
        cmd_path = find_command(argv[0], envp);
        if (!cmd_path)
        {
            fprintf(stderr, "%s: command not found\n", argv[0]);/////change
            return (127);  // Command not found
        }
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return 1;
    }
    else if (pid == 0)
    {
        // Child process
        if (execve(cmd_path, argv, envp) == -1)
        {
            perror("execve");
            exit(1);
        }
    }
    else
    {
        // Parent process
        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid");
            return 1;
        }
        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            return 128 + WTERMSIG(status);
    }
    if (cmd_path != argv[0])
        free(cmd_path);
    return 0;
}

void save_original_io(t_io *io)
{
    io->original_stdin = dup(STDIN_FILENO);
    io->original_stdout = dup(STDOUT_FILENO);
}

void restore_io(t_io *io)
{
    dup2(io->original_stdin, STDIN_FILENO);
    dup2(io->original_stdout, STDOUT_FILENO);
    close(io->original_stdin);
    close(io->original_stdout);
}

void free_command(t_arg *cmd)
{
    if (!cmd)
        return;

    // Free arguments
    for (int i = 0; cmd->arg && cmd->arg[i]; i++)
        free(cmd->arg[i]);
    free(cmd->arg);

    // Free redirections
    for (int i = 0; cmd->red && cmd->red[i]; i++)
        free(cmd->red[i]);
    free(cmd->red);

    // Free the command structure itself
    free(cmd);
}

void free_tokens(t_token *tokens)
{
    t_token *tmp;

    while (tokens)
    {
        tmp = tokens;
        tokens = tokens->next;
        free(tmp->content);
        free(tmp);
    }
}

int main_shell_loop(t_env *env)
{
    char *input;
    t_token *tokens = NULL;
    t_arg *cmd;
    int exit_status = 0;

    while (1)
    {
        input = readline("minishell> ");
        if (!input)
        {
            printf("\nExiting shell.\n");
            return exit_status;
        }
        if (*input)
            add_history(input);
        if (parsing(input, &tokens, env) == 0)
        {
            cmd = ft_arg_new(2);
            if (cmd)
            {
                if (ft_convert_token_to_arg(tokens, cmd, 0) == 0)
                {
                    int result = execute_command(cmd, env, &exit_status);
                    if (result == -1)  // Check for exit command
                    {
                        free_command(cmd);
                        free_tokens(tokens);
                        //free(input);
                        return exit_status;
                    }
                }
                free_command(cmd);
            }
            free_tokens(tokens);
            tokens = NULL;
        }
        //free(input);
    }
    return exit_status;
}

int execute_command(t_arg *cmd, t_env *env, int *exit_status)
{
    t_io io;
    int status = 0;
    int pipe_fd[2];
    pid_t pid;
    int prev_pipe_read = STDIN_FILENO;

    save_original_io(&io);

    while (cmd)
    {
        if (ft_strcmp(cmd->arg[0], "exit") == 0)
        {
            restore_io(&io);
            if (cmd->arg[1])
                *exit_status = ft_atoi(cmd->arg[1]);
            return (-1);  // Special return value to indicate exit
        }
         // Check for empty command
        if (!cmd->arg[0] || cmd->arg[0][0] == '\0')
        {
            printf("minishell: : command not found\n");
            *exit_status = 127;
            cmd = cmd->next;
            continue;
        }
        int heredoc_count = count_heredocs(cmd->red);
        int *heredoc_fds = NULL;
        if (heredoc_count > 0)
        {
            heredoc_fds = handle_heredocs(cmd->red, heredoc_count, env);
            if (!heredoc_fds)
                return 1;
        }

        if (cmd->next)
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe");
                return 1;
            }
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            if (heredoc_fds)
            {
                for (int i = 0; i < heredoc_count; i++)
                    close(heredoc_fds[i]);
                free(heredoc_fds);
            }
            return 1;
        }
        else if (pid == 0) // Child process
        {
            if (prev_pipe_read != STDIN_FILENO)
            {
                dup2(prev_pipe_read, STDIN_FILENO);
                close(prev_pipe_read);
            }
            if (cmd->next)
            {
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }

            if (heredoc_fds)
            {
                dup2(heredoc_fds[heredoc_count - 1], STDIN_FILENO);
                for (int i = 0; i < heredoc_count; i++)
                    close(heredoc_fds[i]);
            }

            if (apply_redirections(cmd->red) == -1)
                exit(1);

            if (is_builtin(cmd->arg[0]))
                return (execute_builtin(cmd, env, exit_status));
            else
                exit(execute_external_command(cmd->arg, env->env_vars));
                // execvp(cmd->arg[0], cmd->arg);
            
            // perror("execvp");
            // exit(1);
        }
        else // Parent process
        {
            if (prev_pipe_read != STDIN_FILENO)
                close(prev_pipe_read);
            
            if (cmd->next)
            {
                close(pipe_fd[1]);
                prev_pipe_read = pipe_fd[0];
            }
            else
            {
                prev_pipe_read = STDIN_FILENO;
            }

            if (heredoc_fds)
            {
                for (int i = 0; i < heredoc_count; i++)
                    close(heredoc_fds[i]);
                free(heredoc_fds);
            }

            if (!cmd->next)
            {
                waitpid(pid, &status, 0);
                *exit_status = WEXITSTATUS(status);
            }

            cmd = cmd->next;
        }
    }

    // Wait for any remaining child processes
    while (wait(NULL) > 0)
        ;

    restore_io(&io);
    return *exit_status;
}

// int execute_command(t_arg *cmd, t_env *env, int *exit_status)//here doc no pip yes
// {
//     t_io io;
//     int status = 0;
//     int pipe_fd[2];
//     pid_t pid;
//     int prev_pipe_read = STDIN_FILENO;

//     save_original_io(&io);

//     while (cmd)
//     {
//         if (cmd->next)
//         {
//             if (pipe(pipe_fd) == -1)
//             {
//                 perror("pipe");
//                 return 1;
//             }
//         }

//         pid = fork();
//         if (pid == -1)
//         {
//             perror("fork");
//             return 1;
//         }
//         else if (pid == 0) // Child process
//         {
//             if (prev_pipe_read != STDIN_FILENO)
//             {
//                 dup2(prev_pipe_read, STDIN_FILENO);
//                 close(prev_pipe_read);
//             }
//             if (cmd->next)
//             {
//                 dup2(pipe_fd[1], STDOUT_FILENO);
//                 close(pipe_fd[0]);
//                 close(pipe_fd[1]);
//             }

//             if (apply_redirections(cmd->red) == -1)
//                 exit(1);

//             if (is_builtin(cmd->arg[0]))
//                 exit(execute_builtin(cmd, env, exit_status));
//             else
//                 execvp(cmd->arg[0], cmd->arg);
            
//             perror("execvp");
//             exit(1);
//         }
//         else // Parent process
//         {
//             if (prev_pipe_read != STDIN_FILENO)
//                 close(prev_pipe_read);
            
//             if (cmd->next)
//             {
//                 close(pipe_fd[1]);
//                 prev_pipe_read = pipe_fd[0];
//             }
//             else
//             {
//                 prev_pipe_read = STDIN_FILENO;
//             }

//             if (!cmd->next)
//             {
//                 waitpid(pid, &status, 0);
//                 *exit_status = WEXITSTATUS(status);
//             }

//             cmd = cmd->next;
//         }
//     }

//     // Wait for any remaining child processes
//     while (wait(NULL) > 0)
//         ;

//     restore_io(&io);
//     return *exit_status;
// }

// int execute_command(t_arg *cmd, t_env *env, int *exit_status)
// {
//     t_io io;
//     int status = 0;
//     int pipe_fd[2];
//     pid_t pid;
//     int prev_pipe_read = STDIN_FILENO;

//     save_original_io(&io);

//     while (cmd)
//     {
//         if (is_builtin(cmd->arg[0]))
//         {
//             status = execute_builtin(cmd, env, exit_status);
//             if (ft_strcmp(cmd->arg[0], "exit") == 0)
//                 exit(status);
//             cmd = cmd->next;
//             continue;
//         }
        
//         if (cmd->next)
//         {
//             if (pipe(pipe_fd) == -1)
//             {
//                 perror("pipe");
//                 return 1;
//             }
//         }

//         int heredoc_count = count_heredocs(cmd->red);
//         int *heredoc_fds = NULL;
//         if (heredoc_count > 0)
//         {
//             heredoc_fds = handle_heredocs(cmd->red, heredoc_count, env);;
//             if (!heredoc_fds)
//                 return 1;
//         }
//         pid = fork();
//         if (pid == -1)
//         {
//             perror("fork");
//             if (heredoc_fds)
//             {
//                 for (int i = 0; i < heredoc_count; i++)
//                     close(heredoc_fds[i]);
//                 free(heredoc_fds);
//             }
//             return 1;
//         }
//         else if (pid == 0) // Child process
//         {
//             if (prev_pipe_read != STDIN_FILENO)
//             {
//                 dup2(prev_pipe_read, STDIN_FILENO);
//                 close(prev_pipe_read);
//             }
//             if (cmd->next)
//             {
//                 dup2(pipe_fd[1], STDOUT_FILENO);
//                 close(pipe_fd[0]);
//                 close(pipe_fd[1]);
//             }
//             if (heredoc_fds)
//             {
//                 dup2(heredoc_fds[heredoc_count - 1], STDIN_FILENO);
//                 for (int i = 0; i < heredoc_count; i++)
//                     close(heredoc_fds[i]);
//             }
//             if (apply_redirections(cmd->red) == -1)
//                 exit(1);

//             if (is_builtin(cmd->arg[0]))
//                 exit(execute_builtin(cmd, env, exit_status));
//             else
//                 execvp(cmd->arg[0], cmd->arg);
            
//             perror("execvp");
//             exit(1);
//         }
//         else // Parent process
//         {
//             if (prev_pipe_read != STDIN_FILENO)
//                 close(prev_pipe_read);
            
//             if (cmd->next)
//             {
//                 close(pipe_fd[1]);
//                 prev_pipe_read = pipe_fd[0];
//             }
//             else
//             {
//                 prev_pipe_read = STDIN_FILENO;
//             }

//             if (heredoc_fds)
//             {
//                 for (int i = 0; i < heredoc_count; i++)
//                     close(heredoc_fds[i]);
//                 free(heredoc_fds);
//             }

//             cmd = cmd->next;
//         }
//     }

//     // Wait for all child processes
//     while (wait(&status) > 0)
//         ;

//     restore_io(&io);
//     *exit_status = WEXITSTATUS(status);
//     return *exit_status;
// }


// int execute_command(t_arg *cmd, t_env *env, int *exit_status)///lastone
// {
//     t_io io;
//     int status = 0;
//     int pipe_fd[2];
//     pid_t pid;
//     int prev_pipe_read = STDIN_FILENO;

//     save_original_io(&io);

//     while (cmd)
//     {
//         if (is_builtin(cmd->arg[0]))
//         {
//             status = execute_builtin(cmd, env, exit_status);
//             if (ft_strcmp(cmd->arg[0], "exit") == 0)
//                 exit(status); // Exit the entire shell process
//             cmd = cmd->next;
//             continue;
//         }
        
//         if (cmd->next) // If there's a next command, we need to pipe
//         {
//             if (pipe(pipe_fd) == -1)
//             {
//                 perror("pipe");
//                 return 1;
//             }
//         }

//         pid = fork();
//         if (pid == -1)
//         {
//             perror("fork");
//             return 1;
//         }
//         else if (pid == 0) // Child process
//         {
//             if (prev_pipe_read != STDIN_FILENO)
//             {
//                 dup2(prev_pipe_read, STDIN_FILENO);
//                 close(prev_pipe_read);
//             }
//             if (cmd->next)
//             {
//                 dup2(pipe_fd[1], STDOUT_FILENO);
//                 close(pipe_fd[0]);
//                 close(pipe_fd[1]);
//             }
//             if (apply_redirections(cmd->red) == -1)
//                 exit(1);

//             if (is_builtin(cmd->arg[0]))
//                 return (execute_builtin(cmd, env, exit_status));
//             else
//                 // return (execute_external_command(cmd->arg, env->env_vars));
//                 execvp(cmd->arg[0], cmd->arg);
            
//             perror("execvp");
//             exit(1);
//         }
//         else // Parent process
//         {
//             if (prev_pipe_read != STDIN_FILENO)
//                 close(prev_pipe_read);
            
//             if (cmd->next)
//             {
//                 close(pipe_fd[1]);
//                 prev_pipe_read = pipe_fd[0];
//             }
//             else
//             {
//                 prev_pipe_read = STDIN_FILENO;
//             }

//             cmd = cmd->next;
//         }
//     }

//     // Wait for all child processes
//     while (wait(&status) > 0)
//         ;

//     restore_io(&io);
//     *exit_status = WEXITSTATUS(status);
//     return *exit_status;
// }

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;

    t_io io;
    save_original_io(&io);

    t_env *env = create_env(envp);
    if (!env)
    {
        fprintf(stderr, "Failed to create environment\n");
        return 1;
    }

    int status = main_shell_loop(env);

    free_env(env);
    restore_io(&io);
    return status;
}

// #include <string.h>
// #include <stdlib.h>
// #include <stdio.h>

// #define MAX_ARGS 100
// #define MAX_REDIRECTIONS 10

// t_arg *parse_command(char *input)
// {
//     t_arg *cmd = malloc(sizeof(t_arg));
//     if (!cmd) {
//         perror("malloc");
//         return NULL;
//     }

//     char *token;
//     char *saveptr;
//     int arg_count = 0;
//     int red_count = 0;

//     cmd->arg = malloc(sizeof(char*) * MAX_ARGS);
//     cmd->red = malloc(sizeof(char*) * MAX_REDIRECTIONS);

//     if (!cmd->arg || !cmd->red) {
//         perror("malloc");
//         free(cmd);
//         return NULL;
//     }

//     token = strtok_r(input, " \t\n", &saveptr);
//     while (token != NULL) {
//         if (strcmp(token, ">") == 0 || strcmp(token, "<") == 0 || 
//             strcmp(token, ">>") == 0 || strcmp(token, "<<") == 0) {
//             // This is a redirection operator
//             if (red_count >= MAX_REDIRECTIONS - 1) {
//                 fprintf(stderr, "Too many redirections\n");
//                 break;
//             }
//             cmd->red[red_count++] = strdup(token);
//             token = strtok_r(NULL, " \t\n", &saveptr);
//             if (token == NULL) {
//                 fprintf(stderr, "Syntax error: missing file for redirection\n");
//                 break;
//             }
//             cmd->red[red_count++] = strdup(token);
//         } else {
//             // This is a command or argument
//             if (arg_count >= MAX_ARGS - 1) {
//                 fprintf(stderr, "Too many arguments\n");
//                 break;
//             }
//             cmd->arg[arg_count++] = strdup(token);
//         }
//         token = strtok_r(NULL, " \t\n", &saveptr);
//     }

//     cmd->arg[arg_count] = NULL;
//     cmd->red[red_count] = NULL;

//     // If no redirections were found, free the redirections array
//     if (red_count == 0) {
//         free(cmd->red);
//         cmd->red = NULL;
//     }
//     // for (int i = 0; cmd->arg[i] != NULL; i++) {
//     //     // printf("%s ", cmd->arg[i]);
//     // }
//     // // printf("\n");
//     // if (cmd->red) {
//     //     for (int i = 0; cmd->red[i] != NULL; i++) {
//     //         // printf("%s ", cmd->red[i]);
//     //     }
//     // } else {
//     //     printf("None");
//     // }
//     // printf("\n");

//     return cmd;
// }
