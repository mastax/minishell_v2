/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <elel-bah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 21:17:09 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/04 11:55:00 by elel-bah         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../mini_shell.h"

#define BUFFER_SIZE 1024

static char *append_buffer(char *line, char *buffer, int bytes_read, int *total_size)
{
    char *new_line;
    int new_size = *total_size + bytes_read + 1;

    new_line = malloc(new_size);
    if (!new_line)
    {
        free(line);
        return NULL;
    }
    if (line)
    {
        ft_memcpy(new_line, line, *total_size);
        free(line);
    }
    ft_memcpy(new_line + *total_size, buffer, bytes_read);
    *total_size = new_size - 1;
    new_line[*total_size] = '\0';
    return new_line;
}

char *read_line(void)
{
    char *line = NULL;
    char buffer[BUFFER_SIZE];
    int bytes_read;
    int total_size = 0;

    while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0) {
        // if (read_line_interrupted)
        // {
        //     free(line);
        //     return NULL;
        // }

        line = append_buffer(line, buffer, bytes_read, &total_size);
        if (!line)
            return NULL;
        if (line[total_size - 1] == '\n') {
            line[total_size - 1] = '\0';
            return line;
        }
    }

    if (bytes_read == -1 || (bytes_read == 0 && total_size == 0)) {
        free(line);
        return NULL;
    }

    return line;
}

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

int count_heredocs(char **red)
{
    int count;
    int i;

    count = 0;
    i = 0;
    if (!red || !(*red))
        return (0);
    while (red[i] != NULL)
    {
        if (ft_strcmp(red[i], "<<") == 0)
            count++;
        i++;
    }
    return count;
}
