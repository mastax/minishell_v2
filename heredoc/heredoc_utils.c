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

// int	ft_expand_herdoc_var(char **var, t_env *env, t_type prv_type, int i)
// {
// 	int		j;
// 	char	*var_name;

// 	while (var[0][i] != '\0')
// 	{
// 		if (var[0][i] == '\'' && check_induble(*var, i) == 0)
// 			i = expanding_helper(*var, i);
// 		else if (var[0][i] == '$' && (is_valid_var(var[0][i + 1]) == 1))
// 		{
// 			j = ft_name_len(*var, i + 1);
// 			var_name = ft_substr(*var, i + 1, j);
// 			if (!var_name)
// 				return (ft_putstr_fd("minishell : malloc error", 2), -1);
// 			if (check_can_expand(var_name, env, prv_type, j, *var) == 0)
// 				return (free(var_name), -1);
// 			if (check_can_expand(var_name, env, prv_type, j, *var) == 1)
// 				if (ft_var_update(i, var, get_var_from_env(var_name, j, env),
// 						&var[0][i + j + 1]) == -1)
// 					return (ft_putstr_fd("minishell : malloc error", 2), -1);
// 		}
// 		else if ((var[0][i] == '$' && (is_valid_var(var[0][i + 1]) != 1))
// 			|| (var[0][i] != '$' && var[0][i] != '\0'))
// 			i++;
// 	}
// 	return (0);
// }

int	ft_expand_herdoc_var(char **var, t_env *env, t_type prv_type, int i)
{
	int		j;
	char	*var_name;

	while (var[0][i] != '\0')
	{
		if (var[0][i] == '\'')
			i = ft_skipe_qoute(*var, i);
		else if (var[0][i] == '$' && (is_valid_var(var[0][i + 1]) == 1))
		{
			j = ft_name_len(*var, i + 1);
			var_name = ft_substr(*var, i + 1, j);
			if (!var_name)
				return (-1);
			if (prv_type == WORD)
				if (ft_var_update(i, var, get_var_from_env(var_name, j, env),
						&var[0][i + j + 1]) == -1)
					return (-1);
		}
		else if ((var[0][i] == '$' && (is_valid_var(var[0][i + 1]) != 1))
			|| (var[0][i] != '$' && var[0][i] != '\0'))
			i++;
	}
	return (0);
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
    if (ft_expand_herdoc_var(&expanded_line, env, WORD, 0) == -1)
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
