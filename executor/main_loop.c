#include "../mini_shell.h"

int setup_pipes(int pipe_count, int pipe_fds[][2])
{
    for (int i = 0; i < pipe_count; i++) {
        if (pipe(pipe_fds[i]) == -1) {
            perror("pipe");
            return 1;
        }
    }
    return 0;
}

static int handle_input(char *input, t_env *env)
{
    (void)env;
    if (!input)
    {
        ft_putstr_fd("\nExiting shell.\n", 1);
        return (1);
    }
    if (*input)
        add_history(input);
    return (0);
}
static int process_command(t_token *tokens, t_env *env, int *exit_status)
{
    t_arg *cmd;
    int result;

    if (!tokens)
        return (0);
    cmd = ft_arg_new(2);
    if (!cmd)
        return (0);
    if (ft_convert_token_to_arg(tokens, cmd, 0) == 0)
    {
        result = execute_command(cmd, env, exit_status);
        free_command(cmd);
        return result;  // Return the result from execute_command
    }
    free_command(cmd);
    return (0);
}

int main_shell_loop(t_env *env)
{
    char *input;
    t_token *tokens;
    int exit_status;

    exit_status = 0;
    sig_init();

    while (1)
    {
        tokens = NULL;
        g_sig.pid = 0;  // Reset pid before each command
        g_sig.sigint = 0;  // Reset sigint flag

        signal(SIGINT, sig_int);
        signal(SIGQUIT, sig_quit);

        input = readline("minishell> ");

        if (input == NULL)
        {
            write(STDERR_FILENO, "exit\n", 6);
            break;
        //     return (exit_status);
        }

        if (g_sig.sigint)//for ctrl-c than echo $?
        {
            //free(input);
            exit_status = g_sig.exit_status;
            g_sig.sigint = 0;  // Reset the flag
           // continue;
        }

        // if (g_sig.sigint)
        // {
        //     // free(input);
        //     continue;  // Skip the rest of the loop if SIGINT was received
        // }


        if (handle_input(input, env))
            return (exit_status);

        int parse_result = parsing(input, &tokens, env, exit_status);
        if (parse_result == 0)
        {
            if (process_command(tokens, env, &exit_status) == -1)
            {
                free_tokens(tokens);
                return (exit_status);
            }
            free_tokens(tokens);
            tokens = NULL;
        }
        else if (parse_result == -1)
        {
            free_tokens(tokens);
            g_sig.exit_status = 1;
            // continue;
        }
        else if (parse_result == 258)
        {
            free_tokens(tokens);
            g_sig.exit_status = 258;
            // continue;
        }

        exit_status = g_sig.exit_status;
    }
    return (exit_status);
}
