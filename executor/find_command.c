#include "../mini_shell.h"

static char *get_path_from_env(char **envp)
{
    int i;

    i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
            return (envp[i] + 5);
        i++;
    }
    return (NULL);
}

static char *create_full_path(char *dir, char *cmd)
{
    char *full_path;

    full_path = malloc(ft_strlen(dir) + ft_strlen(cmd) + 2);
    if (!full_path)
        return (NULL);
    ft_strlcpy(full_path, dir, ft_strlen(dir) + 1);
    ft_strlcat(full_path, "/", ft_strlen(dir) + 2);
    ft_strlcat(full_path, cmd, ft_strlen(dir) + ft_strlen(cmd) + 2);
    return (full_path);
}

static char *search_command_in_path(char *path_copy, char *cmd)
{
    char *dir;
    char *full_path;

    dir = ft_strtok(path_copy, ":");
    while (dir)
    {
        full_path = create_full_path(dir, cmd);
        if (!full_path)
            return (NULL);
        if (access(full_path, X_OK) == 0)
            return (full_path);
        free(full_path);
        dir = ft_strtok(NULL, ":");
    }
    return (NULL);
}

char *find_command(char *cmd, char **envp)
{
    char *path;
    char *path_copy;
    char *full_path;

    path = get_path_from_env(envp);
    if (!path)
        return (NULL);
    path_copy = ft_strdup(path);
    if (!path_copy)
        return (NULL);
    full_path = search_command_in_path(path_copy, cmd);
    free(path_copy);
    return (full_path);
}