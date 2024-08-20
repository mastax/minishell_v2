#include "../mini_shell.h"

// static char *get_path_from_env(char **envp)
// {
//     int i;

//     i = 0;
//     while (envp[i])
//     {
//         if (ft_strncmp(envp[i], "PATH=", 5) == 0)
//             return (envp[i] + 5);
//         i++;
//     }
//     return (NULL);
// }

// static char *create_full_path(char *dir, char *cmd)
// {
//     char *full_path;

//     full_path = malloc(ft_strlen(dir) + ft_strlen(cmd) + 2);
//     if (!full_path)
//         return (NULL);
//     ft_strlcpy(full_path, dir, ft_strlen(dir) + 1);
//     ft_strlcat(full_path, "/", ft_strlen(dir) + 2);
//     ft_strlcat(full_path, cmd, ft_strlen(dir) + ft_strlen(cmd) + 2);
//     return (full_path);
// }

// static char *search_command_in_path(char *path_copy, char *cmd)
// {
//     char *dir;
//     char *full_path;

//     dir = ft_strtok(path_copy, ":");
//     while (dir)
//     {
//         full_path = create_full_path(dir, cmd);
//         if (!full_path)
//             return (NULL);
//         if (access(full_path, X_OK) == 0)
//             return (full_path);
//         free(full_path);
//         dir = ft_strtok(NULL, ":");
//     }
//     return (NULL);
// }

// static char *find_command(char *cmd, char **envp)
// {
//     char *path;
//     char *path_copy;
//     char *full_path;

//     path = get_path_from_env(envp);
//     if (!path)
//         return (NULL);
//     path_copy = ft_strdup(path);
//     if (!path_copy)
//         return (NULL);
//     full_path = search_command_in_path(path_copy, cmd);
//     free(path_copy);
//     return (full_path);
// }
//=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// static char *find_command(char *cmd, char **envp)
// {
//     char *path;
//     char *path_copy;
//     char *dir;
//     char *full_path;

//     path = NULL;
//     path_copy = NULL;
//     full_path = NULL;
//     // Find the PATH variable
//     for (int i = 0; envp[i]; i++)
//     {
//         if (ft_strncmp(envp[i], "PATH=", 5) == 0)
//         {
//             path = envp[i] + 5;
//             break;
//         }
//     }
//     if (!path)
//         return NULL;

//     path_copy = ft_strdup(path);
//     if (!path_copy)
//         return NULL;

//     dir = ft_strtok(path_copy, ":");
//      while (dir)
//     {
//         full_path = malloc(ft_strlen(dir) + ft_strlen(cmd) + 2);
//         if (!full_path)
//         {
//             free(path_copy);
//             return NULL;
//         }
//         ft_strlcpy(full_path, dir, ft_strlen(dir) + 1);
//         ft_strlcat(full_path, "/", ft_strlen(dir) + 2);
//         ft_strlcat(full_path, cmd, ft_strlen(dir) + ft_strlen(cmd) + 2);
        
//         if (access(full_path, X_OK) == 0)
//         {
//             free(path_copy);
//             return full_path;
//         }
        
//         free(full_path);
//         dir = ft_strtok(NULL, ":");
//     }

//     free(path_copy);
//     return NULL;
// }

// static char *find_command(char *cmd, char **envp)
// {
//     char *path;
//     char *path_copy;
//     char *dir;
//     char *full_path;

//     path = NULL;
//     path_copy = NULL;
//     full_path = NULL;
//     // Find the PATH variable
//     for (int i = 0; envp[i]; i++)
//     {
//         if (ft_strncmp(envp[i], "PATH=", 5) == 0)
//         {
//             path = envp[i] + 5;
//             break;
//         }
//     }
//     if (!path)
//         return NULL;

//     path_copy = ft_strdup(path);
//     if (!path_copy)
//         return NULL;

//     dir = ft_strtok(path_copy, ":");
//     while (dir)
//     {
//         full_path = malloc(ft_strlen(dir) + ft_strlen(cmd) + 2);
//         if (!full_path)
//         {
//             free(path_copy);
//             return NULL;
//         }
//         snprintf(full_path, ft_strlen(dir) + ft_strlen(cmd) + 2, "%s/%s", dir, cmd);
//         // snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
//         // sprintf(full_path, "%s/%s", dir, cmd);
        
//         if (access(full_path, X_OK) == 0)
//         {
//             free(path_copy);
//             return full_path;
//         }
        
//         free(full_path);
//         dir = ft_strtok(NULL, ":");
//     }

//     free(path_copy);
//     return NULL;
// }


//=-=-=-=-=-=-==-=--=-=-=-=-=-=-=-=-=---


// static char *get_command_path(char **argv, char **envp)
// {
//     char *cmd_path;

//     if (!argv[0] || argv[0][0] == '\0')
//         return NULL;
//     if (access(argv[0], X_OK) == 0)
//         return (argv[0]);
//     cmd_path = find_command(argv[0], envp);
//     if (!cmd_path)
//     {
//         ft_putstr_fd(argv[0], 2);
//         ft_putstr_fd(": command not found\n", 2);
//     }
//     return (cmd_path);
// }

// static int execute_child_process(char *cmd_path, char **argv, char **envp)
// {
//     if (execve(cmd_path, argv, envp) == -1)
//     {
//         perror("execve");
//         exit(1);
//     }
//     return (0);
// }

// static int handle_parent_process(pid_t pid)
// {
//     int status;

//     if (waitpid(pid, &status, 0) == -1)
//     {
//         perror("waitpid");
//         return (1);
//     }
//     if (WIFEXITED(status))
//         return (WEXITSTATUS(status));
//     else if (WIFSIGNALED(status))
//         return (128 + WTERMSIG(status));
//     return (0);
// }

// int execute_external_command(char **argv, char **envp)
// {
//     pid_t pid;
//     char *cmd_path;
//     int status = 0;

//     // Check if the command is an empty string
//     if (argv[0] == NULL || argv[0][0] == '\0')
//     {
//         ft_putstr_fd(": command not found\n", 2);
//         return (127);  // Command not found
//     }
//     cmd_path = get_command_path(argv, envp);
//     if (!cmd_path)
//         return (127);  // Command not found
//     pid = fork();
//     if (pid == -1)
//     {
//         perror("fork");
//         return (1);
//     }
//     else if (pid == 0)
//     {
//         if (execute_child_process(cmd_path, argv, envp) == -1)
//         {
//             perror("execve");
//             exit(1);
//         }
//     }
//         //status = execute_child_process(cmd_path, argv, envp);
//     else
//         status = handle_parent_process(pid);
//     if (cmd_path != argv[0])
//         free(cmd_path);
//     return (status);
// }

// int execute_external_command(char **argv, char **envp)
// {
//     pid_t pid;
//     int status;
//     char *cmd_path;

//     if (access(argv[0], X_OK) == 0)
//         cmd_path = argv[0];
//     else
//     {
//         cmd_path = find_command(argv[0], envp);
//         if (!cmd_path)
//         {
//             fprintf(stderr, "%s: command not found\n", argv[0]);/////change
//             return (127);  // Command not found
//         }
//     }
//     pid = fork();
//     if (pid == -1)
//     {
//         perror("fork");
//         return 1;
//     }
//     else if (pid == 0)
//     {
//         // Child process
//         if (execve(cmd_path, argv, envp) == -1)
//         {
//             perror("execve");
//             exit(1);
//         }
//     }
//     else
//     {
//         // Parent process
//         if (waitpid(pid, &status, 0) == -1)
//         {
//             perror("waitpid");
//             return 1;
//         }
//         if (WIFEXITED(status))
//             return WEXITSTATUS(status);
//         else if (WIFSIGNALED(status))
//             return 128 + WTERMSIG(status);
//     }
//     if (cmd_path != argv[0])
//         free(cmd_path);
//     return 0;
// }

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--

// void save_original_io(t_io *io)
// {
//     io->original_stdin = dup(STDIN_FILENO);
//     io->original_stdout = dup(STDOUT_FILENO);
// }

// void restore_io(t_io *io)
// {
//     dup2(io->original_stdin, STDIN_FILENO);
//     dup2(io->original_stdout, STDOUT_FILENO);
//     close(io->original_stdin);
//     close(io->original_stdout);
// }

// void free_command(t_arg *cmd)
// {
//     if (!cmd)
//         return;

//     // Free arguments
//     for (int i = 0; cmd->arg && cmd->arg[i]; i++)
//         free(cmd->arg[i]);
//     free(cmd->arg);

//     // Free redirections
//     for (int i = 0; cmd->red && cmd->red[i]; i++)
//         free(cmd->red[i]);
//     free(cmd->red);

//     // Free the command structure itself
//     free(cmd);
// }

// void free_tokens(t_token *tokens)
// {
//     t_token *tmp;

//     while (tokens)
//     {
//         tmp = tokens;
//         tokens = tokens->next;
//         free(tmp->content);
//         free(tmp);
//     }
// }

// void free_env(t_env *env)
// {
//     if (env)
//     {
//         for (int i = 0; env->env_vars[i]; i++)
//             free(env->env_vars[i]);
//         free(env->env_vars);
//         // Free any other members of t_env
//         free(env);
//     }
// }

//=-=-=-=-=-=-=-=-=-==-=--=

// static int handle_input(char *input, t_env *env)
// {
//     (void)env;
//     if (!input)
//     {
//         ft_putstr_fd("\nExiting shell.\n", 1);
//         return (1);
//     }
//     if (*input)
//         add_history(input);
//     return (0);
// }

// static int process_command(t_token *tokens, t_env *env, int *exit_status)
// {
//     t_arg *cmd;
//     int result;

//     cmd = ft_arg_new(2);
//     if (!cmd)
//         return (0);
//     if (ft_convert_token_to_arg(tokens, cmd, 0) == 0)
//     {
//         result = execute_command(cmd, env, exit_status);
//         free_command(cmd);
//         return result;  // Return the result from execute_command
//     }
//     free_command(cmd);
//     return (0);
// }

// static void cleanup(t_token *tokens)
// {
//     free_tokens(tokens);
// }

// int main_shell_loop(t_env *env)
// {
//     char *input;
//     t_token *tokens;
//     int exit_status;

//     exit_status = 0;
//     sig_init();

//     while (1)
//     {
//         tokens = NULL;
//         g_sig.pid = 0;  // Reset pid before each command
//         signal(SIGINT, sig_int);
//         signal(SIGQUIT, sig_quit);

//         input = readline("minishell> ");

//         if (input == NULL)
//         {
//             write(STDERR_FILENO, "exit\n", 6);
//             break;
//         //     return (exit_status);
//         }

//         if (handle_input(input, env))
//             return (exit_status);

//         int parse_result = parsing(input, &tokens, env, exit_status);
//         if (parse_result == 0)
//         {
//             if (process_command(tokens, env, &exit_status) == -1)
//             {
//                 free_tokens(tokens);
//                 return (exit_status);
//             }
//             free_tokens(tokens);
//             tokens = NULL;
//         }
//           else if (parse_result == -1)
//         {
//             free_tokens(tokens);
//             g_sig.exit_status = 2;
//             continue;
//         }

//         exit_status = g_sig.exit_status;
//     }
//     return (exit_status);
// }

// int main_shell_loop(t_env *env)//last
// {
//     char *input;
//     t_token *tokens;
//     int exit_status;

//     exit_status = 0;
//     tokens = NULL;
//     sig_init();

//     while (1)
//     {
//         g_sig.pid = 0;  // Reset pid before each command
//         signal(SIGINT, sig_int);
//         signal(SIGQUIT, sig_quit);  // Set SIGQUIT handler

//         input = readline("minishell> ");
//         if (input == NULL)  // EOF (Ctrl+D)
//         {
//             write(STDERR_FILENO, "\nexit\n", 6);
//             return (exit_status);
//         }
//         if (g_sig.sigint)
//         {
//             g_sig.sigint = 0;
//             continue;
//         }
//         if (handle_input(input, env))
//             return (exit_status);
//         if (parsing(input, &tokens, env) == 0)
//         {
//             if (process_command(tokens, env, &exit_status) == -1)
//             {
//                 cleanup(tokens);
//                 return (exit_status);
//             }
//             cleanup(tokens);
//             tokens = NULL;
//         }
//         exit_status = g_sig.exit_status;  // Update exit_status after each command
//     }
//     return (exit_status);
// }

// int main_shell_loop(t_env *env)// last before the changing
// {
//     char *input;
//     t_token *tokens;
//     int exit_status;

//     exit_status = 0;
//     tokens = NULL;
//     while (1)
//     {
//         input = readline("minishell> ");
//         if (handle_input(input, env))
//             return (exit_status);
//         if (parsing(input, &tokens, env) == 0)
//         {
//             if (process_command(tokens, env, &exit_status))
//             {
//                 cleanup(tokens);
//                 return (exit_status);
//             }
//             cleanup(tokens);
//             tokens = NULL;
//         }
//         // free(input);  // Commented out as in the original
//     }
//     return (exit_status);
// }

//=-=-=-=-=--=-=--==-=-=-=--=
// int main_shell_loop(t_env *env)
// {
//     char *input;
//     t_token *tokens = NULL;
//     t_arg *cmd;
//     int exit_status = 0;

//     while (1)
//     {
//         input = readline("minishell> ");
//         if (!input)
//         {
//             printf("\nExiting shell.\n");
//             return exit_status;
//         }
//         if (*input)
//             add_history(input);
//         if (parsing(input, &tokens, env) == 0)
//         {
//             cmd = ft_arg_new(2);
//             if (cmd)
//             {
//                 if (ft_convert_token_to_arg(tokens, cmd, 0) == 0)
//                 {
//                     int result = execute_command(cmd, env, &exit_status);
//                     if (result == -1)  // Check for exit command
//                     {
//                         free_command(cmd);
//                         free_tokens(tokens);
//                         //ree_env(env);
//                         //free(input);
//                         printf("Exiting shell.\n");
//                         return exit_status;
//                     }
//                 }
//                 free_command(cmd);
//             }
//             free_tokens(tokens);
//             tokens = NULL;
//         }
//        //free(input);
//     }
//     return exit_status;
// }

// int main_shell_loop(t_env *env)
// {
//     char *input;
//     t_token *tokens = NULL;
//     t_arg *cmd;
//     int exit_status = 0;

//     while (1)
//     {
//         input = readline("minishell> ");
//         if (!input)
//         {
//             printf("\nExiting shell.\n");
//             return exit_status;
//         }
//         if (*input)
//             add_history(input);
//         if (parsing(input, &tokens, env) == 0)
//         {
//             cmd = ft_arg_new(2);
//             if (cmd)
//             {
//                 if (ft_convert_token_to_arg(tokens, cmd, 0) == 0)
//                 {
//                     int result = execute_command(cmd, env, &exit_status);
//                     if (result == -1)  // Check for exit command
//                     {
//                         free_command(cmd);
//                         free_tokens(tokens);
//                         //free(input);
//                         return exit_status;
//                     }
//                 }
//                 free_command(cmd);
//             }
//             free_tokens(tokens);
//             tokens = NULL;
//         }
//         //free(input);
//     }
//     return exit_status;
// }

// int main_shell_loop(t_env *env)
// {
//     char *input;
//     t_token *tokens = NULL;
//     t_arg *cmd;
//     int exit_status = 0;

//     while (1)
//     {
//         input = readline("minishell> ");
//         if (!input)
//         {
//             printf("\nExiting shell.\n");
//             return exit_status;
//         }
//         if (*input)
//             add_history(input);
//         if (parsing(input, &tokens, env) == 0)
//         {
//             cmd = ft_arg_new(2);
//             if (cmd)
//             {
//                 if (ft_convert_token_to_arg(tokens, cmd, 0) == 0)
//                 {
//                     int result = execute_command(cmd, env, &exit_status);
//                     if (result == -1)  // Check for exit command
//                     {
//                         free_command(cmd);
//                         free_tokens(tokens);
//                         //free(input);
//                         return exit_status;
//                     }
//                 }
//                 free_command(cmd);
//             }
//             free_tokens(tokens);
//             tokens = NULL;
//         }
//         //free(input);
//     }
//     return exit_status;
// }

// #define MAX_PIPES 100
// #define MAX_COMMANDS 101


//=-=-=-=-=-

// int setup_pipes(int pipe_count, int pipe_fds[][2]) {
//     for (int i = 0; i < pipe_count; i++) {
//         if (pipe(pipe_fds[i]) == -1) {
//             perror("pipe");
//             return 1;
//         }
//     }
//     return 0;
// }

// static void setup_child_process(int cmd_index, int pipe_count, int pipe_fds[][2], int *heredoc_fds, int heredoc_count) {
//     if (cmd_index > 0) {
//         dup2(pipe_fds[cmd_index - 1][0], STDIN_FILENO);
//     }
//     if (cmd_index < pipe_count) {
//         dup2(pipe_fds[cmd_index][1], STDOUT_FILENO);
//     }

//     for (int i = 0; i < pipe_count; i++) {
//         close(pipe_fds[i][0]);
//         close(pipe_fds[i][1]);
//     }

//     if (heredoc_fds) {
//         dup2(heredoc_fds[heredoc_count - 1], STDIN_FILENO);
//         for (int i = 0; i < heredoc_count; i++)
//             close(heredoc_fds[i]);
//         free(heredoc_fds);
//     }
// }

// static void cleanup_parent_process(int *heredoc_fds, int heredoc_count) {
//     if (heredoc_fds) {
//         for (int i = 0; i < heredoc_count; i++)
//             close(heredoc_fds[i]);
//         free(heredoc_fds);
//     }
// }

// static int wait_for_children(pid_t *pids, int command_count, int *exit_status) {
//     for (int i = 0; i < command_count; i++) {
//         int status;
//         waitpid(pids[i], &status, 0);
//         if (i == command_count - 1) {
//             *exit_status = WEXITSTATUS(status);
//             g_sig.exit_status = *exit_status;
//         }
//     }
//     return *exit_status;
// }


//=-=-=-==-=-

// int count_commands(t_arg *cmd)
// {
//     int count = 0;
//     while (cmd)
//     {
//         count++;
//         cmd = cmd->next;
//     }
//     return count;
// }

// static int setup_execution(t_arg *cmd, int *command_count, int *pipe_count, int pipe_fds[][2], t_io *io) {
//     *command_count = count_commands(cmd);
//     *pipe_count = *command_count - 1;

//     save_original_io(io);

//     if (setup_pipes(*pipe_count, pipe_fds) != 0) {
//         restore_io(io);
//         return 1;
//     }
//     return 0;
// }

// static int fork_and_execute(t_arg *cmd, t_env *env, int *exit_status, int cmd_index, int pipe_count, int pipe_fds[][2], int *heredoc_fds, pid_t *pid) {
//     *pid = fork();
//     if (*pid == -1) {
//         perror("fork");
//         return 1;
//     } else if (*pid == 0) { // Child process
//         setup_child_process(cmd_index, pipe_count, pipe_fds, heredoc_fds, count_heredocs(cmd->red));

//         if (apply_redirections(cmd->red) == -1)
//             exit(1);

//         if (is_builtin(cmd->arg[0])) {
//             exit(execute_builtin(cmd, env, exit_status));
//         } else {
//             exit(execute_external_command(cmd->arg, env->env_vars));
//         }
//     }
//     return 0;
// }

// static void cleanup_pipes(int pipe_count, int pipe_fds[][2]) {
//     for (int i = 0; i < pipe_count; i++) {
//         close(pipe_fds[i][0]);
//         close(pipe_fds[i][1]);
//     }
// }
//=-=-=-=-

// static int setup_and_check_heredoc(t_arg *cmd, t_env *env, t_io *io, int *heredoc_count, int **heredoc_fds, int command_count)
// {
//     (void)(command_count);
//     *heredoc_count = count_heredocs(cmd->red);
//     *heredoc_fds = NULL;
//     if (*heredoc_count > 0)
//     {
//         *heredoc_fds = handle_heredocs(cmd->red, *heredoc_count, env);
//         if (!*heredoc_fds)
//         {
//             restore_io(io);
//             return 1;
//         }
//     }
//     return 0;
// }

// static int execute_commands_loop(t_arg *cmd, t_env *env, int *exit_status, int command_count, int pipe_count, int pipe_fds[][2], pid_t *pids, t_io *io)
// {
//     (void)command_count;
//     int cmd_index = 0;
//     while (cmd)
//     {
//         int heredoc_count;
//         int *heredoc_fds;
//         int setup_result = setup_and_check_heredoc(cmd, env, io, &heredoc_count, &heredoc_fds, command_count);
//         if (setup_result != 0)
//             return setup_result;

//         if (fork_and_execute(cmd, env, exit_status, cmd_index, pipe_count, pipe_fds, heredoc_fds, &pids[cmd_index]) != 0)
//         {
//             restore_io(io);
//             return 1;
//         }

//         cleanup_parent_process(heredoc_fds, heredoc_count);
//         cmd = cmd->next;
//         cmd_index++;
//     }
//     return 0;
// }

// int execute_command(t_arg *cmd, t_env *env, int *exit_status)
// {
//     t_io io;
//     int command_count, pipe_count;
//     int pipe_fds[MAX_PIPES][2];
//     pid_t pids[MAX_COMMANDS];

//     if (setup_execution(cmd, &command_count, &pipe_count, pipe_fds, &io) != 0)
//         return 1;

//     // Add this check for single builtin command
//     if (command_count == 1 && is_builtin(cmd->arg[0]))
//     {
//         if (apply_redirections(cmd->red) == -1)
//         {
//         restore_io(&io);
//         return 1;
//         }
//         int result = execute_builtin(cmd, env, exit_status);
//         restore_io(&io);
//         return result;
//     }

//     int result = execute_commands_loop(cmd, env, exit_status, command_count, pipe_count, pipe_fds, pids, &io);
//     if (result != 0)
//         return result;

//     cleanup_pipes(pipe_count, pipe_fds);
//     result = wait_for_children(pids, command_count, exit_status);
//     restore_io(&io);
//     return result;
// }

// =-=-=-=-
// int execute_command(t_arg *cmd, t_env *env, int *exit_status) {
//     t_io io;
//     int command_count, pipe_count;
//     int pipe_fds[MAX_PIPES][2];
//     pid_t pids[MAX_COMMANDS];

//     if (setup_execution(cmd, &command_count, &pipe_count, pipe_fds, &io) != 0) {
//         return 1;
//     }

//     int cmd_index = 0;
//     while (cmd)
//     {
//         int heredoc_count = count_heredocs(cmd->red);
//         int *heredoc_fds = NULL;
//         if (heredoc_count > 0)
//         {
//             heredoc_fds = handle_heredocs(cmd->red, heredoc_count, env);
//             if (!heredoc_fds)
//             {
//                 restore_io(&io);
//                 return 1;
//             }
//         }
//         else if (command_count == 1 && is_builtin(cmd->arg[0])) {
//             int result = execute_builtin(cmd, env, exit_status);
//             restore_io(&io);
//             return result;
//         }

//         if (fork_and_execute(cmd, env, exit_status, cmd_index, pipe_count, pipe_fds, heredoc_fds, &pids[cmd_index]) != 0) {
//             restore_io(&io);
//             return 1;
//         }

//         cleanup_parent_process(heredoc_fds, heredoc_count);

//         cmd = cmd->next;
//         cmd_index++;
//     }

//     cleanup_pipes(pipe_count, pipe_fds);

//     int result = wait_for_children(pids, command_count, exit_status);

//     restore_io(&io);
//     return result;
// }

// int execute_command(t_arg *cmd, t_env *env, int *exit_status) {
//     t_io io;
//     int command_count, pipe_count;
//     int pipe_fds[MAX_PIPES][2];
//     pid_t pids[MAX_COMMANDS];

//     if (setup_execution(cmd, &command_count, &pipe_count, pipe_fds, &io) != 0) {
//         return 1;
//     }

//     if (execute_commands(cmd, env, exit_status, pipe_count, pipe_fds, pids) != 0) {
//         restore_io(&io);
//         return 1;
//     }
//     int single_builtin_result = handle_single_builtin(cmd, env, exit_status, &io);
//     if (single_builtin_result != -1) {
//         return single_builtin_result;
//     }


//     cleanup_pipes(pipe_count, pipe_fds);

//     int result = wait_for_children(pids, command_count, exit_status);

//     restore_io(&io);
//     return result;
// }

//-=-=-=-=-=--=
// int execute_command(t_arg *cmd, t_env *env, int *exit_status) {
//     t_io io;
//     int command_count = count_commands(cmd);
//     int pipe_count = command_count - 1;
//     int pipe_fds[pipe_count][2];
//     pid_t pids[command_count];

//     save_original_io(&io);

//     // Create all pipes
//     for (int i = 0; i < pipe_count; i++) {
//         if (pipe(pipe_fds[i]) == -1) {
//             perror("pipe");
//             restore_io(&io);
//             return 1;
//         }
//     }

//     int cmd_index = 0;
//     while (cmd)
//     {
//         // Execute builtin in parent process if it's the only command
//         int heredoc_count = count_heredocs(cmd->red);
//         int *heredoc_fds = NULL;
//         if (heredoc_count > 0)
//         {
//             heredoc_fds = handle_heredocs(cmd->red, heredoc_count, env);
//             if (!heredoc_fds)
//             {
//                 restore_io(&io);
//                 return 1;
//             }
//         }

//         else if (command_count == 1 && is_builtin(cmd->arg[0])) {
//             int result = execute_builtin(cmd, env, exit_status);
//             restore_io(&io);
//             return result;
//         }

//         pids[cmd_index] = fork();
//         if (pids[cmd_index] == -1)
//         {
//             perror("fork");
//             restore_io(&io);
//             return 1;
//         }
//         else if (pids[cmd_index] == 0) // Child process
//         {
//             // Set up pipes
//             if (cmd_index > 0) {
//                 dup2(pipe_fds[cmd_index - 1][0], STDIN_FILENO);
//             }
//             if (cmd_index < pipe_count) {
//                 dup2(pipe_fds[cmd_index][1], STDOUT_FILENO);
//             }

//             // Close all pipe file descriptors
//             for (int i = 0; i < pipe_count; i++) {
//                 close(pipe_fds[i][0]);
//                 close(pipe_fds[i][1]);
//             }

//             // Handle heredocs
//             if (heredoc_fds)
//             {
//                 dup2(heredoc_fds[heredoc_count - 1], STDIN_FILENO);
//                 for (int i = 0; i < heredoc_count; i++)
//                     close(heredoc_fds[i]);
//                 free(heredoc_fds);
//             }

//             // Apply redirections
//             if (apply_redirections(cmd->red) == -1)
//                 exit(1);

//             // Execute command
//             if (is_builtin(cmd->arg[0]))
//             {
//                 exit(execute_builtin(cmd, env, exit_status));
//             }
//             else
//             {
//                 exit(execute_external_command(cmd->arg, env->env_vars));
//             }
//         }

//         // Parent process
//         if (heredoc_fds)
//         {
//             for (int i = 0; i < heredoc_count; i++)
//                 close(heredoc_fds[i]);
//             free(heredoc_fds);
//         }

//         cmd = cmd->next;
//         cmd_index++;
//     }

//     // Close all pipe file descriptors in parent
//     for (int i = 0; i < pipe_count; i++) {
//         close(pipe_fds[i][0]);
//         close(pipe_fds[i][1]);
//     }

//     // Wait for all child processes
//     for (int i = 0; i < command_count; i++) {
//         int status;
//         waitpid(pids[i], &status, 0);
//         if (i == command_count - 1) {
//             *exit_status = WEXITSTATUS(status);
//             g_sig.exit_status = *exit_status;
//         }
//     }

//     restore_io(&io);
//     return *exit_status;
// }

// int execute_command(t_arg *cmd, t_env *env, int *exit_status)//////last
// {
//     t_io io;
//     int command_count = count_commands(cmd);
//     int pipe_count = command_count - 1;
//     int pipe_fds[pipe_count][2];
//     pid_t pids[command_count];

//     save_original_io(&io);

//     // Create all pipes
//     for (int i = 0; i < pipe_count; i++) {
//         if (pipe(pipe_fds[i]) == -1) {
//             perror("pipe");
//             restore_io(&io);
//             return 1;
//         }
//     }

//     int cmd_index = 0;
//     while (cmd)
//     {
//         int heredoc_count = count_heredocs(cmd->red);
//         int *heredoc_fds = NULL;
//         if (heredoc_count > 0)
//         {
//             heredoc_fds = handle_heredocs(cmd->red, heredoc_count, env);
//             if (!heredoc_fds)
//             {
//                 restore_io(&io);
//                 return 1;
//             }
//         }

//         pids[cmd_index] = fork();
//         if (pids[cmd_index] == -1)
//         {
//             perror("fork");
//             restore_io(&io);
//             return 1;
//         }
//         else if (pids[cmd_index] == 0) // Child process
//         {
//             // Set up pipes
//             if (cmd_index > 0) {
//                 dup2(pipe_fds[cmd_index - 1][0], STDIN_FILENO);
//             }
//             if (cmd_index < pipe_count) {
//                 dup2(pipe_fds[cmd_index][1], STDOUT_FILENO);
//             }

//             // Close all pipe file descriptors
//             for (int i = 0; i < pipe_count; i++) {
//                 close(pipe_fds[i][0]);
//                 close(pipe_fds[i][1]);
//             }

//             // Handle heredocs
//             if (heredoc_fds)
//             {
//                 dup2(heredoc_fds[heredoc_count - 1], STDIN_FILENO);
//                 for (int i = 0; i < heredoc_count; i++)
//                     close(heredoc_fds[i]);
//                 free(heredoc_fds);
//             }

//             // Apply redirections
//             if (apply_redirections(cmd->red) == -1)
//                 exit(1);

//             // Execute command
//             if (is_builtin(cmd->arg[0]))
//             {
//                 exit(execute_builtin(cmd, env, exit_status));
//             }
//             else
//             {
//                 exit(execute_external_command(cmd->arg, env->env_vars));
//             }
//         }

//         // Parent process
//         if (heredoc_fds)
//         {
//             for (int i = 0; i < heredoc_count; i++)
//                 close(heredoc_fds[i]);
//             free(heredoc_fds);
//         }

//         cmd = cmd->next;
//         cmd_index++;
//     }

//     // Close all pipe file descriptors in parent
//     for (int i = 0; i < pipe_count; i++) {
//         close(pipe_fds[i][0]);
//         close(pipe_fds[i][1]);
//     }

//     // Wait for all child processes
//     for (int i = 0; i < command_count; i++) {
//         int status;
//         waitpid(pids[i], &status, 0);
//         if (i == command_count - 1) {
//             *exit_status = WEXITSTATUS(status);
//             g_sig.exit_status = *exit_status;
//         }
//     }

//     restore_io(&io);
//     return *exit_status;
// }

// int execute_command(t_arg *cmd, t_env *env, int *exit_status)//1111
// {
//     t_io io;
//     int status = 0;
//     int pipe_fd[2];
//     int prev_pipe_read = STDIN_FILENO;

//     save_original_io(&io);

//     while (cmd)
//     {
//         int heredoc_count = count_heredocs(cmd->red);
//         int *heredoc_fds = NULL;
//         if (heredoc_count > 0)
//         {
//             heredoc_fds = handle_heredocs(cmd->red, heredoc_count, env);
//             if (!heredoc_fds)
//             {
//                 restore_io(&io);
//                 return 1;
//             }
//         }

//         if (is_builtin(cmd->arg[0]))
//         {
//             // Handle heredoc for builtin commands
//             if (heredoc_fds)
//             {
//                 dup2(heredoc_fds[heredoc_count - 1], STDIN_FILENO);
//                 for (int i = 0; i < heredoc_count; i++)
//                     close(heredoc_fds[i]);
//                 free(heredoc_fds);
//             }

//             // Execute builtin in the parent process
//             *exit_status = execute_builtin(cmd, env, exit_status);
//             if (ft_strcmp(cmd->arg[0], "exit") == 0)
//             {
//                 restore_io(&io);
//                 return -1;  // Special return value to indicate exit
//             }
//         }
//         else
//         {
//             // For non-builtin commands, continue with forking
//             if (cmd->next)
//             {
//                 if (pipe(pipe_fd) == -1)
//                 {
//                     perror("pipe");
//                     if (heredoc_fds)
//                     {
//                         for (int i = 0; i < heredoc_count; i++)
//                             close(heredoc_fds[i]);
//                         free(heredoc_fds);
//                     }
//                     restore_io(&io);
//                     return 1;
//                 }
//             }

//             g_sig.pid = fork();
//             if (g_sig.pid == -1)
//             {
//                 perror("fork");
//                 if (heredoc_fds)
//                 {
//                     for (int i = 0; i < heredoc_count; i++)
//                         close(heredoc_fds[i]);
//                     free(heredoc_fds);
//                 }
//                 restore_io(&io);
//                 return 1;
//             }
//             else if (g_sig.pid == 0) // Child process
//             {
//                 if (prev_pipe_read != STDIN_FILENO)
//                 {
//                     dup2(prev_pipe_read, STDIN_FILENO);
//                     close(prev_pipe_read);
//                 }
//                 if (cmd->next)
//                 {
//                     dup2(pipe_fd[1], STDOUT_FILENO);
//                     close(pipe_fd[0]);
//                     close(pipe_fd[1]);
//                 }

//                 if (heredoc_fds)
//                 {
//                     dup2(heredoc_fds[heredoc_count - 1], STDIN_FILENO);
//                     for (int i = 0; i < heredoc_count; i++)
//                         close(heredoc_fds[i]);
//                     free(heredoc_fds);
//                 }

//                 if (apply_redirections(cmd->red) == -1)
//                     exit(1);

//                 exit(execute_external_command(cmd->arg, env->env_vars));
//             }
//             else // Parent process
//             {
//                 if (prev_pipe_read != STDIN_FILENO)
//                     close(prev_pipe_read);

//                 if (cmd->next)
//                 {
//                     close(pipe_fd[1]);
//                     prev_pipe_read = pipe_fd[0];
//                 }
//                 else
//                 {
//                     waitpid(g_sig.pid, &status, 0);
//                     *exit_status = WEXITSTATUS(status);
//                     g_sig.exit_status = *exit_status;
//                 }

//                 if (heredoc_fds)
//                 {
//                     for (int i = 0; i < heredoc_count; i++)
//                         close(heredoc_fds[i]);
//                     free(heredoc_fds);
//                 }
//             }
//         }
//         cmd = cmd->next;
//     }

//     // Wait for any remaining child processes
//     while (wait(NULL) > 0)
//         ;

//     restore_io(&io);
//     return *exit_status;
// }

// int execute_command(t_arg *cmd, t_env *env, int *exit_status)//last_has the problem of heredoc
// {
//     t_io io;
//     int status = 0;
//     int pipe_fd[2];
//     int prev_pipe_read = STDIN_FILENO;

//     save_original_io(&io);

//     while (cmd)
//     {
//         if (is_builtin(cmd->arg[0]))
//         {
//             // Execute builtin in the parent process
//             *exit_status = execute_builtin(cmd, env, exit_status);
//             if (ft_strcmp(cmd->arg[0], "exit") == 0)
//             {
//                 restore_io(&io);
//                 return -1;  // Special return value to indicate exit
//             }
//         }
//         else
//         {
//             // For non-builtin commands, continue with forking
//             if (cmd->next)
//             {
//                 if (pipe(pipe_fd) == -1)
//                 {
//                     perror("pipe");
//                     return 1;
//                 }
//             }

//             g_sig.pid = fork();
//             if (g_sig.pid == -1)
//             {
//                 perror("fork");
//                 return 1;
//             }
//             else if (g_sig.pid == 0) // Child process
//             {
//                 if (prev_pipe_read != STDIN_FILENO)
//                 {
//                     dup2(prev_pipe_read, STDIN_FILENO);
//                     close(prev_pipe_read);
//                 }
//                 if (cmd->next)
//                 {
//                     dup2(pipe_fd[1], STDOUT_FILENO);
//                     close(pipe_fd[0]);
//                     close(pipe_fd[1]);
//                 }

//                 if (apply_redirections(cmd->red) == -1)
//                     exit(1);

//                 exit(execute_external_command(cmd->arg, env->env_vars));
//             }
//             else // Parent process
//             {
//                 if (prev_pipe_read != STDIN_FILENO)
//                     close(prev_pipe_read);

//                 if (cmd->next)
//                 {
//                     close(pipe_fd[1]);
//                     prev_pipe_read = pipe_fd[0];
//                 }
//                 else
//                 {
//                     waitpid(g_sig.pid, &status, 0);
//                     *exit_status = WEXITSTATUS(status);
//                     g_sig.exit_status = *exit_status;
//                 }
//             }
//         }
//         cmd = cmd->next;
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
//     // pid_t *pids = NULL;
//     // pid_t pid;
//     int prev_pipe_read = STDIN_FILENO;

//     save_original_io(&io);

//     while (cmd)
//     {
//         if (ft_strcmp(cmd->arg[0], "exit") == 0)
//         {
//             restore_io(&io);
//             if (cmd->arg[1])
//                 *exit_status = ft_atoi(cmd->arg[1]);
//             return (-1);  // Special return value to indicate exit
//         }

//         //  Check for empty command
//         if (!cmd->arg[0] || cmd->arg[0][0] == '\0')
//         {
//             printf("minishell: : command not found\n");
//             *exit_status = 127;
//             cmd = cmd->next;
//             continue;
//         }
//         int heredoc_count = count_heredocs(cmd->red);
//         int *heredoc_fds = NULL;
//         if (heredoc_count > 0)
//         {
//             heredoc_fds = handle_heredocs(cmd->red, heredoc_count, env);
//             if (!heredoc_fds)
//                 return 1;
//         }

//         if (cmd->next)
//         {
//             if (pipe(pipe_fd) == -1)
//             {
//                 perror("pipe");
//                 return 1;
//             }
//         }

//         g_sig.pid = fork();
//         if (g_sig.pid == -1)
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
//         else if (g_sig.pid == 0) // Child process
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
//             {
//                 return(execute_builtin(cmd, env, exit_status));
//             }
//             else
//                 exit(execute_external_command(cmd->arg, env->env_vars));
//         }
//         else // Parent process
//         {
//             // pids[i++] = g_sig.pid;

//             if (prev_pipe_read != STDIN_FILENO)
//                 close(prev_pipe_read);

//             if (cmd->next)
//             {
//                 close(pipe_fd[1]);
//                 prev_pipe_read = pipe_fd[0];
//             }
//             else
//             {
//                 close(pipe_fd[0]);
//                 close(pipe_fd[1]);
//             }
//             if (!cmd->next)
//             {
//                 waitpid(g_sig.pid, &status, 0);
//                 *exit_status = WEXITSTATUS(status);
//                 g_sig.exit_status = *exit_status;
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

// int execute_command(t_arg *cmd, t_env *env, int *exit_status) // fix the problem of pip and heredoc but pwd cant exit after
// {
//     t_io io;
//     int status = 0;
//     int pipe_fd[2];
//     pid_t pid;
//     int prev_pipe_read = STDIN_FILENO;
//     int cmd_count = 0;
//     pid_t *pids = NULL;

//     save_original_io(&io);

//     // Count commands and allocate pid array
//     for (t_arg *tmp = cmd; tmp; tmp = tmp->next)
//         cmd_count++;
//     pids = malloc(sizeof(pid_t) * cmd_count);
//     if (!pids) {
//         perror("malloc");
//         return 1;
//     }

//     int i = 0;
//     while (cmd)
//     {
//         if (ft_strcmp(cmd->arg[0], "exit") == 0)
//         {
//             restore_io(&io);
//             if (cmd->arg[1])
//                 *exit_status = ft_atoi(cmd->arg[1]);
//             free(pids);
//             return (-1);  // Special return value to indicate exit
//         }
        
//         // Check for empty command
//         if (!cmd->arg[0] || cmd->arg[0][0] == '\0')
//         {
//             printf("minishell: : command not found\n");
//             *exit_status = 127;
//             cmd = cmd->next;
//             continue;
//         }
//         int heredoc_count = count_heredocs(cmd->red);
//         int *heredoc_fds = NULL;
//         if (heredoc_count > 0)
//         {
//             heredoc_fds = handle_heredocs(cmd->red, heredoc_count, env);
//             if (!heredoc_fds)
//                 return 1;
//         }

//         if (cmd->next)
//         {
//             if (pipe(pipe_fd) == -1)
//             {
//                 perror("pipe");
//                 free(pids);
//                 return 1;
//             }
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
//                 return(execute_builtin(cmd, env, exit_status));
//             else
//                 exit(execute_external_command(cmd->arg, env->env_vars));
//         }
//         else // Parent process
//         {
//             pids[i++] = pid;

//             if (prev_pipe_read != STDIN_FILENO)
//                 close(prev_pipe_read);

//             if (cmd->next)
//             {
//                 close(pipe_fd[1]);
//                 prev_pipe_read = pipe_fd[0];
//             }
//             else
//             {
//                 close(pipe_fd[0]);
//                 close(pipe_fd[1]);
//             }

//             cmd = cmd->next;
//         }
//     }

//     // Wait for all child processes
//     for (int j = 0; j < i; j++)
//     {
//         waitpid(pids[j], &status, 0);
//         if (WIFEXITED(status))
//             *exit_status = WEXITSTATUS(status);
//     }

//     free(pids);
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
//         if (ft_strcmp(cmd->arg[0], "exit") == 0)
//         {
//             restore_io(&io);
//             if (cmd->arg[1])
//                 *exit_status = ft_atoi(cmd->arg[1]);
//             return (-1);  // Special return value to indicate exit
//         }
        
//         //  Check for empty command
//         if (!cmd->arg[0] || cmd->arg[0][0] == '\0')
//         {
//             printf("minishell: : command not found\n");
//             *exit_status = 127;
//             cmd = cmd->next;
//             continue;
//         }
//         int heredoc_count = count_heredocs(cmd->red);
//         int *heredoc_fds = NULL;
//         if (heredoc_count > 0)
//         {
//             heredoc_fds = handle_heredocs(cmd->red, heredoc_count, env);
//             if (!heredoc_fds)
//                 return 1;
//         }

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
//             {
//                 int builtin_result = execute_builtin(cmd, env, exit_status);
//                 exit(builtin_result);
//             }
//             else
//                 exit(execute_external_command(cmd->arg, env->env_vars));
//         }
//         else // Parent process
//         {
//             // Your existing parent process code...

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
//         if (ft_strcmp(cmd->arg[0], "exit") == 0)
//         {
//             restore_io(&io);
//             if (cmd->arg[1])
//                 *exit_status = ft_atoi(cmd->arg[1]);
//             return (-1);  // Special return value to indicate exit
//         }
//          // Check for empty command
//         if (!cmd->arg[0] || cmd->arg[0][0] == '\0')
//         {
//             printf("minishell: : command not found\n");
//             *exit_status = 127;
//             cmd = cmd->next;
//             continue;
//         }
//         int heredoc_count = count_heredocs(cmd->red);
//         int *heredoc_fds = NULL;
//         if (heredoc_count > 0)
//         {
//             heredoc_fds = handle_heredocs(cmd->red, heredoc_count, env);
//             if (!heredoc_fds)
//                 return 1;
//         }

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
//                 return (execute_builtin(cmd, env, exit_status));
//             else
//                 return(execute_external_command(cmd->arg, env->env_vars));
//                 // execvp(cmd->arg[0], cmd->arg);
            
//             // perror("execvp");
//             // exit(1);
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
// t_sig g_sig;

// int main(int argc, char **argv, char **envp)
// {
//     rl_catch_signals = 0;
//     (void)argc;
//     (void)argv;

//     t_io io;
//     save_original_io(&io);

//     t_env *env = create_env(envp);
//     if (!env)
//     {
//         fprintf(stderr, "Failed to create environment\n");
//         return 1;
//     }

//     int status = main_shell_loop(env);

//     //free_env(env);
//     restore_io(&io);
//     return status;
// }
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
