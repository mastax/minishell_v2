#include "../mini_shell.h"

int pwd(int fd)
{
    char current_wd[PATH_MAX];

    if (getcwd(current_wd, sizeof(current_wd)) == NULL)
    {
        // ft_putendl_fd(current_wd, STDOUT_FILENO);
        return (1);
    }
    ft_putendl_fd(current_wd, fd);
    return (0);
}
