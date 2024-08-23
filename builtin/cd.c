#include "../mini_shell.h"

int ft_change_dir(char **av, t_env *env, int *exit_status)
{
    char current_dir[PATH_MAX];
    char new_dir[PATH_MAX];

    if (getcwd(current_dir, sizeof(current_dir)) == NULL)
    {
        ft_putstr_fd("cd: error retrieving current directory: getcwd:", 2);
        ft_putstr_fd(" access parent directories: No such file or directory\n", 2);
    }
    if (!av[1])
        *exit_status = change_to_home(env);
    else if (ft_strcmp(av[1], "-") == 0)
        *exit_status = change_to_previous(env);
    else
        *exit_status = chdir(av[1]);
    if (*exit_status == -1)
    {
        perror("cd");
        return (*exit_status = 1);
    }
    // if (*exit_status == -1)
    // {
    //     perror("cd");
    //     *exit_status = 1;
    //     return (1);
    // }
    if (getcwd(new_dir, sizeof(new_dir)) == NULL)
        return (1);
    set_env_value(env, "OLDPWD", current_dir);
    set_env_value(env, "PWD", new_dir);
    return (0);
}
