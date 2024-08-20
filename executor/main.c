#include "../mini_shell.h"

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

void restore_io(t_io *io)
{
    dup2(io->original_stdin, STDIN_FILENO);
    dup2(io->original_stdout, STDOUT_FILENO);
    close(io->original_stdin);
    close(io->original_stdout);
}

void save_original_io(t_io *io)
{
    io->original_stdin = dup(STDIN_FILENO);
    io->original_stdout = dup(STDOUT_FILENO);
}

int main(int ac, char **av, char **envp)
{
    (void)ac;
    (void)av;
    rl_catch_signals = 0;

    t_io io;
    save_original_io(&io);

    t_env *env = create_env(envp);
    if (!env)
    {
        fprintf(stderr, "Failed to create environment\n");
        return 1;
    }

    int status = main_shell_loop(env);

    //free_env(env);
    restore_io(&io);
    return status;
}