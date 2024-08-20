/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:54:16 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/08/19 20:13:42 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

// char *ft_add_char(char *s, unsigned int index, char to_add)
// {
//     size_t len = strlen(s);
//     char *new_str = malloc(len + 2); // +1 for new char, +1 for null terminator
//     if (!new_str) return NULL;

//     strncpy(new_str, s, index);
//     new_str[index] = to_add;
//     strcpy(new_str + index + 1, s + index);

//     free(s);
//     return new_str;
// }

char	*ft_add_char(char *s, unsigned int index, char to_add)
{
	unsigned int	j;
	int				i;
	char			*str;

	i = 0;
	j = 0;
	if (ft_strlen(s) < index || !s)
		return (NULL);
	str = malloc(sizeof(char) * (ft_strlen(s) + 2));
	if (!str)
		return ((write(2, "Error\nadd_char can't alloc\n", 28)), free(s), NULL);
	while (s[i] != '\0')
	{
		if (j != index)
		{
			str[j] = s[i];
			i++;
		}
		else
			str[j] = to_add;
		j++;
	}
	str[j] = '\0';
	free(s);
	return (str);
}

char *ft_handl_appand_herdoc(char *line, int i)
{
    int len = strlen(line);

    // Check if we have enough characters to perform the operation
    if (i > 0 && i + 1 < len) {
        if ((line[i - 1] != ' ' && line[i - 1] != '\t') &&
            (line[i + 2] < len && line[i + 2] != ' ' && line[i + 2] != '\t'))
        {
            line = ft_add_char(line, i, ' ');
            if (!line) return NULL;
            line = ft_add_char(line, i + 3, ' ');
            if (!line) return NULL;
        }
        else if (line[i - 1] != ' ' && line[i - 1] != '\t')
        {
            line = ft_add_char(line, i, ' ');
            if (!line) return NULL;
        }
        else if (i + 2 < len && line[i + 2] != ' ' && line[i + 2] != '\t')
        {
            line = ft_add_char(line, i + 2, ' ');
            if (!line) return NULL;
        }
    }
    return line;
}

// char	*ft_handl_appand_herdoc(char *line, int i)
// {
// 	if ((line[i - 1] != ' ' && line[i - 1] != '\t')
// 		&& (line[i + 2] != ' ' && line[i + 2] != '\t'))
// 	{
// 		line = ft_add_char(line, i, ' ');
// 		line = ft_add_char(line, i + 3, ' ');
// 	}
// 	else if (line[i - 1] != ' ' && line[i - 1] != '\t')
// 	{
// 		line = ft_add_char(line, i, ' ');
// 	}
// 	else if (line[i + 2] != ' ' && line[i + 2] != '\t')
// 	{
// 		line = ft_add_char(line, i + 2, ' ');
// 	}
// 	return (line);
// }

char	*ft_handl_spc_opr(char *line, int i)
{
	if ((line[i - 1] != ' ' || line[i - 1] != '\t')
		&& (line[i + 1] != ' ' || line[i + 1] != '\t'))
	{
		line = ft_add_char(line, i, ' ');
		line = ft_add_char(line, i + 2, ' ');
	}
	else if (line[i - 1] != ' ' && line[i - 1] != '\t')
	{
		line = ft_add_char(line, i, ' ');
	}
	else if (line[i + 1] != ' ' && line[i + 1] != '\t')
	{
		line = ft_add_char(line, i + 1, ' ');
	}
	return (line);
}

// char *ft_add_space(char *line)
// {
//     int i = 0;
//     while (line && line[i])
//     {
//         if (line[i] == '"' || line[i] == '\'')
//             i = ft_skipe_qoute(line, i) - 1;
//         else if ((line[i] == '>' && line[i + 1] == '>') || (line[i] == '<' && line[i + 1] == '<'))
//         {
//             line = ft_handl_appand_herdoc(line, i);
//             if (!line) return NULL;
//             i += 2;
//         }
//         else if (is_spc_opr(line[i]) == 1)
//         {
//             line = ft_handl_spc_opr(line, i);
//             if (!line) return NULL;
//             i++;
//         }
//         i++;
//     }
//     return line;
// }

char	*ft_add_space(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '"' || line[i] == '\'')
			i = ft_skipe_qoute(line, i) - 1;
		else if ((line[i] == '>' && line[i + 1] == '>') || (line[i] == '<'
				&& line[i + 1] == '<'))
		{
			line = ft_handl_appand_herdoc(line, i);
			if (!line)
				return (NULL);
			i += 2;
		}
		else if (is_spc_opr(line[i]) == 1)
		{
			line = ft_handl_spc_opr(line, i);
			if (!line)
				return (NULL);
			i++;
		}
		i++;
	}
	return (line);
}

static char	*print_type(t_type type)
{
	if (WORD == type)
		return ("WORD");
	else if (PIPE == type)
		return ("PIPE");
	else if (RED_IN == type)
		return ("RED_IN");
	else if (RED_OUT == type)
		return ("RED_OUT");
	else if (HER_DOC == type)
		return ("HER_DOC");
	else if (APPEND == type)
		return ("APPEND");
	else
		return (NULL);
}

int	parsing(char *line, t_token	**token, t_env *env, int exit_status)
{
	t_token	*tmp;

	line = ft_add_space(line);
	if (ft_check_qoutes(line) == -1)
		return (-1);
	line = ft_compress_spaces(line);
	if (get_token(token, line, 1) == -1)
		return (-1);
	if (*token == NULL)
		return (0);
	if (expanding(token, env, exit_status, WORD) == -1)
		return (-1);
	if ((*token)->content[ft_skipe_spaces((*token)->content, 0)] == '\0')
		*token = (*token)->next;
		// return (free(line), 0);
	tmp = *token;
	while (tmp)
	{
		if (tmp->qout_rm == true)
			tmp->content = ft_remove_quotes(tmp->content);
		printf("2!!  Token : {%s}------->>>>>>>> Type : [%s]\n", tmp->content, print_type(tmp->type));
		tmp = tmp->next;
	}
	return (free(line), ft_check_error(*token));
}
