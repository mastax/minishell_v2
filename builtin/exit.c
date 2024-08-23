#include "../mini_shell.h"

int ft_is_arg_nbr(char *ar)
{
    if (*ar == '+' || *ar == '-')
        ar++;
    if (!*ar)
        return (0);
    while (*ar)
    {
        if (*ar < '0' || *ar > '9')
            return (0);
        ar++;
    }
    return (1);
}

int ft_exit(char **av, int *exit_status)
{
    long long exit_code;
    char *endptr;

    printf("exit\n");
    if (!av[1])
        exit(*exit_status);

    exit_code = strtoll(av[1], &endptr, 10);

    if (*endptr != '\0' || av[1][0] == '\0')
    {
        ft_putstr_fd("exit: ", 2);
        ft_putstr_fd(av[1], 2);
        ft_putstr_fd(": numeric argument required\n", 2);
        exit(255);
    }

    if (av[2])
    {
        ft_putstr_fd("exit: too many arguments\n", 2);
        *exit_status = 1;
        return (1);
    }

    exit((int)(exit_code & 0xFF));
}