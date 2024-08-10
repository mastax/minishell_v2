#include "../mini_shell.h"

char *read_line(void)
{
    char *line;
    size_t len;
    ssize_t nread;

    line = NULL;
    len = 0;
    nread = getline(&line, &len, stdin);
    if (nread == -1) {
        free(line);
        return NULL;
    }
    // Remove newline character if present
    if (nread > 0 && line[nread - 1] == '\n')
        line[nread - 1] = '\0';
    return (line);
}

int expand_variable(char **line, t_env *env)
{
    char *expanded_line;

    expanded_line = ft_strdup(*line);
    if (expanded_line == NULL)
    {
        perror("ft_strdup");
        free(*line);
        return -1;
    }
    if (ft_expand_variable(&expanded_line, env, WORD, 0) == -1)
    {
        free(expanded_line);
        free(*line);
        return -1;
    }

    free(*line);
    *line = expanded_line;
    return 0;
}

int write_to_pipe(int pipefd[2], char *line)
{
    write(pipefd[1], line, ft_strlen(line));
    write(pipefd[1], "\n", 1);
    return 0;
}
