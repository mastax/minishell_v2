#include "../mini_shell.h"

// static void cleanup_pipes(int pipe_count, int pipe_fds[][2])
// {
//     for (int i = 0; i < pipe_count; i++) {
//         close(pipe_fds[i][0]);
//         close(pipe_fds[i][1]);
//     }
// }
// //=-=-=-=-=

// static int setup_execution(setup_execution_params *params) {
//     *(params->command_count) = count_commands(params->cmd);
//     *(params->pipe_count) = *(params->command_count) - 1;

//     save_original_io(params->io);

//     if (setup_pipes(*(params->pipe_count), params->pipe_fds) != 0) {
//         restore_io(params->io);
//         return 1;
//     }
//     return 0;
// }

static int setup_and_check_heredoc(heredoc_setup_params *params) {
    (void)(params->command_count);
    *(params->heredoc_count) = count_heredocs(params->cmd->red);
    *(params->heredoc_fds) = NULL;
    if (*(params->heredoc_count) > 0)
    {
        *(params->heredoc_fds) = handle_heredocs(params->cmd->red, *(params->heredoc_count), params->env);
        if (!*(params->heredoc_fds)) {
            restore_io(params->io);
            return 1;
        }
    }
    return 0;
}

int execute_commands_loop(execute_loop_params *params) {
    int cmd_index = 0;
    while (params->cmd) {
        int heredoc_count;
        int *heredoc_fds;

        // In execute_commands_loop function:
        heredoc_setup_params heredoc_params = {
        params->cmd, params->env, params->io, &heredoc_count, &heredoc_fds, params->command_count
        };
        int setup_result = setup_and_check_heredoc(&heredoc_params);
        // int setup_result = setup_and_check_heredoc(params->cmd, params->env, params->io, &heredoc_count, &heredoc_fds, params->command_count);
        if (setup_result != 0)
            return setup_result;

        fork_execute_params fork_params = {
            params->cmd, params->env, params->exit_status, cmd_index,
            params->pipe_count, params->pipe_fds, heredoc_fds, &(params->pids[cmd_index])
        };
        if (fork_and_execute(&fork_params) != 0) {
            restore_io(params->io);
            return 1;
        }

        cleanup_parent_process(heredoc_fds, heredoc_count);
        params->cmd = params->cmd->next;
        cmd_index++;
    }
    return 0;
}
//=-=-=--=

int execute_command(t_arg *cmd, t_env *env, int *exit_status) {//works fine in the case of exit status and 
    t_io io;
    int command_count, pipe_count;
    int pipe_fds[MAX_PIPES][2];
    pid_t pids[MAX_COMMANDS];
    int child_count = 0;

    command_count = count_commands(cmd);
    pipe_count = command_count - 1;

    save_original_io(&io);

    if (setup_pipes(pipe_count, pipe_fds) != 0) {
        restore_io(&io);
        return 1;
    }

    // First, process all heredocs
    t_arg *current_cmd = cmd;
    while (current_cmd) {
        int heredoc_count = count_heredocs(current_cmd->red);
        if (heredoc_count > 0) {
            current_cmd->heredoc_fds = handle_heredocs(current_cmd->red, heredoc_count, env);
            if (!current_cmd->heredoc_fds) {
                restore_io(&io);
                return 1;
            }
        }
        current_cmd = current_cmd->next;
    }

    // Now execute commands
    current_cmd = cmd;
    int cmd_index = 0;
    while (current_cmd) {
        if (current_cmd->arg != NULL && is_builtin(current_cmd->arg[0])) {
            // Execute the builtin in the parent process
            execute_builtin(current_cmd, env, exit_status);
        } else {
            pids[cmd_index] = fork();
            if (pids[cmd_index] == -1) {
                perror("fork");
                restore_io(&io);
                return 1;
            } else if (pids[cmd_index] == 0) { // Child process
                // Set up pipes, heredoc, and other redirections
                // Execute the command
                exit(0);
            }
            child_count++;
        }

        // Parent process
        if (current_cmd->heredoc_fds) {
            for (int i = 0; i < count_heredocs(current_cmd->red); i++) {
                close(current_cmd->heredoc_fds[i]);
            }
        }

        current_cmd = current_cmd->next;
        cmd_index++;
    }

    // Close all pipe fds in parent
    for (int i = 0; i < pipe_count; i++) {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
    }

    // Wait for all child processes
    int result = 0;
    if (child_count > 0) {
        result = wait_for_children(pids, child_count, exit_status);
    }

    restore_io(&io);
    return result;
}

//=-=-=-
// int execute_command(t_arg *cmd, t_env *env, int *exit_status) {//last work with signals
//     t_io io;
//     int command_count, pipe_count;
//     int pipe_fds[MAX_PIPES][2];
//     pid_t pids[MAX_COMMANDS];

//     command_count = count_commands(cmd);
//     pipe_count = command_count - 1;

//     save_original_io(&io);

//     if (setup_pipes(pipe_count, pipe_fds) != 0) {
//         restore_io(&io);
//         return 1;
//     }

//     // First, process all heredocs
//     t_arg *current_cmd = cmd;
//     while (current_cmd) {
//         int heredoc_count = count_heredocs(current_cmd->red);
//         if (heredoc_count > 0) {
//             current_cmd->heredoc_fds = handle_heredocs(current_cmd->red, heredoc_count, env);
//             if (!current_cmd->heredoc_fds) {
//                 restore_io(&io);
//                 return 1;
//             }
//         }
//         current_cmd = current_cmd->next;
//     }

//     // Now execute commands
//     current_cmd = cmd;
//     int cmd_index = 0;
//     while (current_cmd) {
//         pids[cmd_index] = fork();
//         if (pids[cmd_index] == -1) {
//             perror("fork");
//             restore_io(&io);
//             return 1;
//         } else if (pids[cmd_index] == 0) { // Child process
//             // Set up pipes
//             if (cmd_index > 0) {
//                 dup2(pipe_fds[cmd_index - 1][0], STDIN_FILENO);
//             }
//             if (cmd_index < pipe_count) {
//                 dup2(pipe_fds[cmd_index][1], STDOUT_FILENO);
//             }

//             // Close all pipe fds in child
//             for (int i = 0; i < pipe_count; i++) {
//                 close(pipe_fds[i][0]);
//                 close(pipe_fds[i][1]);
//             }

//             // Set up heredoc if it exists
//           int j = count_heredocs(current_cmd->red);
//         if (j > 0 && current_cmd->heredoc_fds) {
//                 dup2(current_cmd->heredoc_fds[0], STDIN_FILENO);
//                 int i = 0;
//                 while (j > i)
//                     close(current_cmd->heredoc_fds[i++]);
//             }

//             // Apply other redirections
//             if (apply_redirections(current_cmd->red) == -1) {
//                 exit(1);
//             }

//             // Execute the command
//             if (current_cmd->arg != NULL && is_builtin(current_cmd->arg[0])) {
//                 exit(execute_builtin(current_cmd, env, exit_status));
//             } else {
//                 if (current_cmd->arg != NULL) {
//                     char *cmd_path = find_command(current_cmd->arg[0], env->env_vars);
//                     if (!cmd_path) {
//                         ft_putstr_fd(current_cmd->arg[0], 2);
//                         ft_putstr_fd(": command not found\n", 2);
//                         exit(127);
//                     }
//                     execve(cmd_path, current_cmd->arg, env->env_vars);
//                     perror("execve");
//                     exit(1);
//                 }
//                 exit(0);
//             }
//         }
//         // Parent process
//         if (current_cmd->heredoc_fds) {
//             for (int i = 0; i < count_heredocs(current_cmd->red); i++) {
//                 close(current_cmd->heredoc_fds[i]);
//             }
//         }

//         current_cmd = current_cmd->next;
//         cmd_index++;
//     }

//     // Close all pipe fds in parent
//     for (int i = 0; i < pipe_count; i++) {
//         close(pipe_fds[i][0]);
//         close(pipe_fds[i][1]);
//     }

//     // Wait for all child processes
//     int result = wait_for_children(pids, command_count, exit_status);

//     restore_io(&io);
//     return result;
// }

// int wait_for_children(pid_t *pids, int command_count, int *exit_status) {
//     int status;
//     int result = 0;

//     for (int i = 0; i < command_count; i++) {
//         if (waitpid(pids[i], &status, 0) == -1) {
//             perror("waitpid");
//             return 1;
//         }
//         if (WIFEXITED(status)) {
//             if (i == command_count - 1) { // Last command determines the exit status
//                 *exit_status = WEXITSTATUS(status);
//             }
//             result = WEXITSTATUS(status);
//         } else if (WIFSIGNALED(status)) {
//             if (i == command_count - 1) { // Last command determines the exit status
//                 *exit_status = 128 + WTERMSIG(status);
//             }
//             result = 128 + WTERMSIG(status);
//         }
//     }

//     return result;
// }

//=-=-=-=


// int execute_command(t_arg *cmd, t_env *env, int *exit_status) {
//     t_io io;
//     int command_count, pipe_count;
//     int pipe_fds[MAX_PIPES][2];
//     pid_t pids[MAX_COMMANDS];

//     command_count = count_commands(cmd);
//     pipe_count = command_count - 1;

//     save_original_io(&io);

//     if (setup_pipes(pipe_count, pipe_fds) != 0) {
//         restore_io(&io);
//         return 1;
//     }

//     // First, process all heredocs
//     t_arg *current_cmd = cmd;
//     while (current_cmd) {
//         int heredoc_count = count_heredocs(current_cmd->red);
//         if (heredoc_count > 0) {
//             current_cmd->heredoc_fds = handle_heredocs(current_cmd->red, heredoc_count, env);
//             if (!current_cmd->heredoc_fds) {
//                 restore_io(&io);
//                 return 1;
//             }
//         }
//         current_cmd = current_cmd->next;
//     }

//     // Now execute commands
//     current_cmd = cmd;
//     int cmd_index = 0;
//     while (current_cmd) {
//         pids[cmd_index] = fork();
//         if (pids[cmd_index] == -1) {
//             perror("fork");
//             restore_io(&io);
//             return 1;
//         } else if (pids[cmd_index] == 0) { // Child process
//             // Set up pipes
//             if (cmd_index > 0) {
//                 dup2(pipe_fds[cmd_index - 1][0], STDIN_FILENO);
//             }
//             if (cmd_index < pipe_count) {
//                 dup2(pipe_fds[cmd_index][1], STDOUT_FILENO);
//             }

//             // Close all pipe fds in child
//             for (int i = 0; i < pipe_count; i++) {
//                 close(pipe_fds[i][0]);
//                 close(pipe_fds[i][1]);
//             }

//             // Set up heredoc if it exists
//             int j = count_heredocs(current_cmd->red);
//             if (j > 0 && current_cmd->heredoc_fds) {
//                 dup2(current_cmd->heredoc_fds[0], STDIN_FILENO);
//                 int i = 0;
//                 while (j > i)
//                     close(current_cmd->heredoc_fds[i++]);
//                 // for (int i = 0; i < heredoc_count; i++) {
//                 // }
//             }

//             // Apply other redirections

//             if (apply_redirections(current_cmd->red) == -1) {
//                 exit(1);
//             }

//             // Execute the command
//             if (current_cmd->arg != NULL && is_builtin(current_cmd->arg[0])) {
//                 exit(execute_builtin(current_cmd, env, exit_status));
           
//             } else {
//                 if (current_cmd->arg != NULL){
//                     char *cmd_path = find_command(current_cmd->arg[0], env->env_vars);
//                 if (!cmd_path) {
//                     ft_putstr_fd(current_cmd->arg[0], 2);
//                     ft_putstr_fd(": command not found\n", 2);
//                     exit(127);
//                 }
//                 execve(cmd_path, current_cmd->arg, env->env_vars);
//                 perror("execve");
//                 exit(1);
//                 }
//                 exit (0);
//             }
//         }
//         // Parent process
//         if (current_cmd->heredoc_fds) {
//             for (int i = 0; i < count_heredocs(current_cmd->red); i++) {
//                 close(current_cmd->heredoc_fds[i]);
//             }
//             // free(current_cmd->heredoc_fds);
//         }

//         current_cmd = current_cmd->next;
//         cmd_index++;
//     }

//     // Close all pipe fds in parent
//     for (int i = 0; i < pipe_count; i++) {
//         close(pipe_fds[i][0]);
//         close(pipe_fds[i][1]);
//     }

//     // Wait for all child processes
//     for (int i = 0; i < command_count; i++) {
//         int status;
//         waitpid(pids[i], &status, 0);
//         if (i == command_count - 1) { // Last command determines the exit status
//             if (WIFEXITED(status)) {
//                 *exit_status = WEXITSTATUS(status);
//             } else if (WIFSIGNALED(status)) {
//                 *exit_status = 128 + WTERMSIG(status);
//             }
//         }
//     }

//     restore_io(&io);
//     return *exit_status;
// }

// int execute_command(t_arg *cmd, t_env *env, int *exit_status) {
//     t_io io;
//     int command_count, pipe_count;
//     int pipe_fds[MAX_PIPES][2];
//     pid_t pids[MAX_COMMANDS];

//     setup_execution_params setup_params = {
//         cmd, &command_count, &pipe_count, pipe_fds, &io
//     };
//     if (setup_execution(&setup_params) != 0) {
//         return 1;
//     }

//     int cmd_index = 0;
//     while (cmd)
//     {
//         printf ("hello1\n");
//         int heredoc_count = count_heredocs(cmd->red);
//         int *heredoc_fds = NULL;
//         printf ("hello1\n");
//         if (heredoc_count > 0)
//         {
//             printf ("hello2\n");

//             heredoc_fds = handle_heredocs(cmd->red, heredoc_count, env);
//             if (!heredoc_fds)
//             {
//                 restore_io(&io);
//                 return 1;
//             }
//         }
//         else if (command_count == 1 && is_builtin(cmd->arg[0])) {
//         printf ("hello2\n");

//             if (apply_redirections(cmd->red) == -1) {
//                 restore_io(&io);
//                 return 1;
//             }
//             int result = execute_builtin(cmd, env, exit_status);
//             restore_io(&io);
//             return result;
//         }
//         printf ("hello2\n");

//         fork_execute_params fork_params = {
//             cmd, env, exit_status, cmd_index, pipe_count, pipe_fds, heredoc_fds, &pids[cmd_index]
//         };
//         if (fork_and_execute(&fork_params) != 0) {
//             restore_io(&io);
//             return 1;
//         }
//         printf ("hello3\n");

//        g_sig.pid = pids[cmd_index];////////

//         cleanup_parent_process(heredoc_fds, heredoc_count);

//         cmd = cmd->next;
//         cmd_index++;
//     }

//     cleanup_pipes(pipe_count, pipe_fds);

//     int result = wait_for_children(pids, command_count, exit_status);

//     // g_sig.exit_status = result;//////exitsssttatus

//     restore_io(&io);
//     return result;
// }
