#include "../mini_shell.h"

int ft_is_arg_nbr(char *ar)
{
    while (*ar)
    {
        if (is_number(*ar) == 0)
            return (0);
        ar++;
    }
    return (1);
}

int		ft_exit(char **av, int status)
{
	ft_putendl_fd("exit", STDERR_FILENO);
	if (!av[1])
		exit(status);
	if (ft_is_arg_nbr(av[1]))
	{
		if (av[1] && av[2])
		{
            perror("too many arguments!");
			// print_error("exit", NULL, "too many arguments!");
			return (1);
		}
		exit(ft_atoi(av[1]));
	}
    perror("numeric argument required!");
	// print_error("exit", av[1], "numeric argument required!");
	exit(255);
	return (0);
}
