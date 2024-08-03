#include "../../mini_shell.h"

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

t_arg *tokens_to_arg(t_token *tokens)
{
    t_arg *cmd = NULL;
    t_arg *current = NULL;
    t_token *token = tokens;
    int arg_count = 0;
    int red_count = 0;

    while (token)
    {
        if (token->type == PIPE || !cmd)
        {
            t_arg *new_cmd = ft_arg_new(2);  // Assuming 2 is the default file descriptor
            if (!new_cmd)
                return NULL;
            if (!cmd)
                cmd = new_cmd;
            else
                ft_argadd_back(&cmd, new_cmd);
            current = new_cmd;
            arg_count = 0;
            red_count = 0;
        }

        if (token->type == WORD)
        {
            current->arg = realloc(current->arg, sizeof(char *) * (arg_count + 2));
            current->arg[arg_count++] = ft_strdup(token->content);
            current->arg[arg_count] = NULL;
        }
        else if (token->type == RED_IN || token->type == RED_OUT || token->type == APPEND || token->type == HER_DOC)
        {
            current->red = realloc(current->red, sizeof(char *) * (red_count + 3));
            current->red[red_count++] = ft_strdup(token->content);
            token = token->next;
            if (token)
                current->red[red_count++] = ft_strdup(token->content);
            current->red[red_count] = NULL;
        }

        token = token->next;
    }

    return cmd;
}

int main_shell_loop(t_env *env)
{
    char *input;
    t_token *tokens = NULL;
    t_arg *cmd;
    int exit_status = 0;

    while (1)
    {
        // Display prompt and read input
        input = readline("minishell> ");
        // Check for EOF (Ctrl+D)
        if (!input)
        {
            printf("\nExiting shell.\n");
            return exit_status;
        }
        // Add input to history
        if (*input)
            add_history(input);
        
        // Parse the input
        if (parsing(input, &tokens, env) == 0)
        {
            // free(input);
            // continue;
            cmd = ft_arg_new(2);
            if (cmd)
            {
                if (ft_convert_token_to_arg(tokens, cmd, 0) == 0)
                {
                // Execute the command
                    exit_status = execute_command(cmd, env, &exit_status);
                // Free the command structure
                    free_command(cmd);
                }

                // Free the tokens
                free_tokens(tokens);
                tokens = NULL;
        
                // Free the input string
                //free(input);
            }
        }
        else
            free(input);
        // Convert tokens to t_arg structure
    }
    return exit_status;
}

// int main_shell_loop(t_env *env)
// {
//     char *input;
//     t_token *tokens = NULL;
//     t_arg *cmd;
//     int exit_status = 0;

//     while (1)
//     {
//         // Display prompt and read input
//         input = readline("minishell> ");
//         // Check for EOF (Ctrl+D)
//         if (!input)
//         {
//             printf("\nExiting shell.\n");
//             return exit_status;
//         }
//         // Add input to history
//         if (*input)
//             add_history(input);
        
//         // Parse the input
//         if (parsing(input, &tokens, env->env_vars) == -1)
//         {
//             free(input);
//             continue;
//         }
        
//         // Convert tokens to t_arg structure
//         cmd = tokens_to_arg(tokens);
//         if (cmd)
//         {
//             // Execute the command
//             exit_status = execute_command(cmd, env, &exit_status);
//             // Free the command structure
//             free_command(cmd);
//         }
        
//         // Free the tokens
//         free_tokens(tokens);
//         tokens = NULL;
        
//         // Free the input string
//         //free(input);
        
//         // Check if we need to exit the shell
//         if (exit_status == -1)
//         {
//             printf("Exiting shell.\n");
//             return 0;
//         }
//     }
//     return exit_status;
// }

// int main_shell_loop(t_env *env)
// {
//     char *input;
//     char **pipeline_commands;
//     t_arg *cmd;
//     int exit_status = 0;

//     while (1)
//     {
//         // Display prompt and read input
//         input = readline("minishell> ");

//         // Check for EOF (Ctrl+D)
//         if (!input)
//         {
//             printf("\nExiting shell.\n");
//             return exit_status;
//         }

//         // Add input to history
//         if (*input)
//             add_history(input);

//         // Check for pipeline
//         pipeline_commands = split_pipeline(input);

//         if (pipeline_commands[1] != NULL) // We have a pipeline
//         {
//             exit_status = handle_pipeline(pipeline_commands, env);
//         }
//         else // Single command
//         {
//             // Parse the input
//             cmd = parse_command(pipeline_commands[0]);

//             if (cmd)
//             {
//                 // Execute the command
//                 exit_status = execute_command(cmd, env, &exit_status);

//                 // Free the command structure
//                 free_command(cmd);
//             }
//         }

//         // Free the pipeline commands
//         free_argv(pipeline_commands);

//         // Free the input string
//         free(input);

//         // Check if we need to exit the shell
//         if (exit_status == -1)
//         {
//             printf("Exiting shell.\n");
//             return 0;
//         }
//     }

//     return exit_status;
// }
// char *reconstruct_full_command(t_arg *cmd);
// int execute_command(t_arg *cmd, t_env *env, int *exit_status)
// {
//     t_io io;
//     int status = 0;
//     char **pipeline_commands;

//     save_original_io(&io);

//     // Reconstruct the full command from all linked t_arg nodes
//     char *full_command = reconstruct_full_command(cmd);
//     if (!full_command)
//         return 1;

//     // Split the command into pipeline segments
//     pipeline_commands = split_pipeline(full_command);
//     free(full_command);

//     if (!pipeline_commands)
//         return 1;

//     if (pipeline_commands[1] == NULL) // No pipe, single command
//     {
//         if (apply_redirections(cmd->red) == -1)
//         {
//             restore_io(&io);
//             free_argv(pipeline_commands);
//             return 1;
//         }

//         if (is_builtin(cmd->arg[0]))
//             status = execute_builtin(cmd, env, exit_status);
//         else
//             status = execute_external_command(cmd->arg, env->env_vars);
//     }
//     else // Pipeline
//     {
//         status = handle_pipeline(pipeline_commands, env);
//         *exit_status = status;
//     }

//     free_argv(pipeline_commands);
//     restore_io(&io);
//     return status;
// }

// // Helper function to reconstruct the full command from all t_arg nodes
// char *reconstruct_full_command(t_arg *cmd)
// {
//     t_arg *current = cmd;
//     int total_len = 0;
    
//     // Calculate total length needed
//     while (current)
//     {
//         for (int i = 0; current->arg[i]; i++)
//             total_len += strlen(current->arg[i]) + 1; // +1 for space or pipe
//         current = current->next;
//         if (current)
//             total_len++; // for the pipe character
//     }
    
//     char *full_command = malloc(total_len + 1); // +1 for null terminator
//     if (!full_command)
//         return NULL;
    
//     full_command[0] = '\0';
//     current = cmd;
//     while (current)
//     {
//         for (int i = 0; current->arg[i]; i++)
//         {
//             strcat(full_command, current->arg[i]);
//             if (current->arg[i + 1])
//                 strcat(full_command, " ");
//         }
//         current = current->next;
//         if (current)
//             strcat(full_command, "|");
//     }
//     return full_command;
// }

int execute_command(t_arg *cmd, t_env *env, int *exit_status)
{
    t_io io;
    int status = 0;
    int pipe_fd[2];
    pid_t pid;

    save_original_io(&io);

    while (cmd)
    {
        if (cmd->next) // If there's a next command, we need to pipe
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
            return 1;
        }
        else if (pid == 0) // Child process
        {
            if (cmd->next)
            {
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }

            if (apply_redirections(cmd->red) == -1)
                exit(1);

            if (is_builtin(cmd->arg[0]))
                // exit(execute_builtin(cmd, env, exit_status));
                return (execute_builtin(cmd, env, exit_status));

            else
                return (execute_external_command(cmd->arg, env->env_vars));
        }
        else // Parent process
        {
            if (cmd->next)
            {
                dup2(pipe_fd[0], STDIN_FILENO);
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }

            cmd = cmd->next;
        }
    }

    // Wait for all child processes
    while (wait(&status) > 0)
        ;

    restore_io(&io);
    *exit_status = WEXITSTATUS(status);
    return *exit_status;
}

// int execute_command(t_arg *cmd, t_env *env, int *exit_status)
// {
//     t_io io;
//     int status = 0;

//     save_original_io(&io);

//     while (cmd)
//     {
//         if (apply_redirections(cmd->red) == -1)
//         {
//             restore_io(&io);
//             return 1;
//         }

//         if (is_builtin(cmd->arg[0]))
//             status = execute_builtin(cmd, env, exit_status);
//         else
//             status = execute_external_command(cmd->arg, env->env_vars);

//         restore_io(&io);
//         cmd = cmd->next;

//         // if (cmd)
//         //     pipe(???);  // You need to implement piping here
//     }

//     return status;
// }

// int execute_command(t_arg *cmd, t_env *env, int *exit_status)
// {
//     if (!cmd->arg || !cmd->arg[0])
//         return 0;

//     int stdin_copy = dup(STDIN_FILENO);
//     int stdout_copy = dup(STDOUT_FILENO);

//     // Apply redirections
//     if (apply_redirections(cmd->red) == -1)
//     {
//         *exit_status = 1;
//         return 1;
//     }

//     if (is_builtin(cmd->arg[0]))
//     {
//         // Execute builtin
//         *exit_status = execute_builtin(cmd, env, exit_status);
//     }
//     else
//     {
//         pid_t pid = fork();
//         if (pid == 0)
//         {
//             // Child process
//             execvp(cmd->arg[0], cmd->arg);
//             perror("execvp");
//             exit(1);
//         }
//         else if (pid > 0)
//         {
//             // Parent process
//             int status;
//             waitpid(pid, &status, 0);
//             *exit_status = WEXITSTATUS(status);
//         }
//         else
//         {
//             perror("fork");
//             *exit_status = 1;
//         }
//     }

//     // Restore original stdin and stdout
//     dup2(stdin_copy, STDIN_FILENO);
//     dup2(stdout_copy, STDOUT_FILENO);
//     close(stdin_copy);
//     close(stdout_copy);

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
