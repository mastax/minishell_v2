#include "../mini_shell.h"

static int is_valid_n_flag(const char *arg)
{
	int i;

    if (arg[0] != '-')
        return 0;
    i = 1;
    while (arg[i] == 'n')
        i++;
    return (arg[i] == '\0');
}

int ft_echo(char **av)
{
    int i = 1;
    int flag_n = 0;

    while (av[i] != NULL && is_valid_n_flag(av[i]))
    {
        flag_n = 1;
        i++;
    }
    while (av[i] != NULL)
    {
        ft_putstr_fd(av[i], STDOUT_FILENO);
        if (av[i + 1] != NULL)
            write(STDOUT_FILENO, " ", 1);
        i++;
    }
    if (flag_n == 0)
        write(1, "\n", 1);
    return (0);
}
