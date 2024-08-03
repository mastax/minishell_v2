#include "../mini_shell.h"

int ft_change_dir(char **av, t_env *env)
{
    int status;
    char current_dir[PATH_MAX];
    char new_dir[PATH_MAX];

    if (getcwd(current_dir, sizeof(current_dir)) == NULL)
        return (1);
    if (!av[1])
        status = change_to_home(env);
    else if (ft_strcmp(av[1], "-") == 0)
        status = change_to_previous(env);
    else
        status = chdir(av[1]);
    if (status == -1)
    {
        perror("cd");
        return (1);
    }
    if (getcwd(new_dir, sizeof(new_dir)) == NULL)
        return (1);
    set_env_value(env, "OLDPWD", current_dir);
    set_env_value(env, "PWD", new_dir);
    return (0);
}
