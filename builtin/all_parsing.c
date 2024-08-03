t_arg	*ft_arglast(t_arg	*lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
	{
		lst = lst->next;
	}
	return (lst);
}

void	ft_argadd_back(t_arg **lst, t_arg *new)
{
	t_arg	*last;

	if (!lst || !new)
		return ;
	last = ft_arglast(*lst);
	if (last != NULL)
		last->next = new;
	else
		*lst = new;
}
t_arg	*ft_arg_new(int fd)
{
	t_arg	*arg;

	arg = malloc(sizeof(t_arg));
	if (!arg)
		return (ft_putstr_fd("Error\n can't malloc for new_arg\n", fd), NULL);
	arg->arg = NULL;
	arg->red = NULL;
	arg->next = NULL;
	return (arg);
}

int	cheack_pipe(t_token *token, t_token *next_token)
{
	if (token->type == PIPE && next_token->type == PIPE)
		return (1);
	else
		return (0);
}

int	cheack_redirect(t_token *token, t_token *next_token)
{
	if (token->type == RED_IN || token->type == RED_OUT
		|| token->type == HER_DOC || token->type == APPEND)
	{
		if (next_token->type == RED_IN || next_token->type == RED_OUT
			|| next_token->type == HER_DOC || next_token->type == APPEND)
			return (1);
		return (0);
	}
	else
		return (0);
}

int	ft_cheack_error(t_token *token)
{
	if (token->type == PIPE)
		return (printf("syntax error near unexpected token '%s'\n"
					, token->content), 1);
	while (token->next)
	{
		if (cheack_pipe(token, token->next) != 0
			|| cheack_redirect(token, token->next) != 0)
			return (printf("syntax error near unexpected token '%s'\n"
					, token->next->content), 1);
		token = token->next;
	}
	if (token->type != WORD)
		return (printf("syntax error near unexpected token 'newline'\n"), 1);
	return (0);
}

int	ft_strncmp(char *s1, char *s2, size_t n)
{
	size_t	i;
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	while (i < n && (str1[i] != '\0' && str2[i] != '\0'))
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	return (0);
}

int	ft_name_len(char *var, int i)
{
	int len;

	len = 0;
	while (var[i + len] != '\0' && (is_valid_var(var[i + len]) == 1))
		len++;
	return (len);
}

char	*get_var_from_env(char *var, int var_len, char **env)
{
	int i;

	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], var, var_len) == 0 && env[i][var_len] == '=')
			return (&env[i][var_len + 1]);
		i++;
	}
	return (NULL);
}

int	ft_var_update(int i, char **var, char *to_update, char	*secend_part)
{
	char	*first_part;
	char	*update_var;

	if (i == 0 && !to_update)
		first_part = ft_strdup("");
	else if (i == 0 && to_update)
		first_part = ft_strdup(to_update);
	else if (i != 0 && !to_update)
		first_part = ft_strndup(*var, i);
	else if (i != 0 && to_update)
		first_part = ft_strjoin(ft_strndup(*var, i), to_update);
	if (!first_part)
		return (-1);
	update_var = ft_strjoin(first_part, secend_part);
	if (!update_var)
	{
		free(first_part);
		return (-1);
	}
	free(*var);
	*var = update_var;
	return (0);
}

int	ft_expand_variable(char **var, char **env, t_type prv_type, int i)
{
	int		j;
	char	*var_name;

	while (var[0][i] != 0)
	{
		if (var[0][i] == '\'')
			i += ft_skipe_qoute(*var, i) + 1;
		else if (var[0][i] == '$' && (is_valid_var(var[0][i + 1]) == 1))
		{
			j = ft_name_len(*var, i + 1);
			var_name = ft_substr(*var, i + 1, j);
			if (!var_name)
				return (-1);
			if (prv_type == WORD || (get_var_from_env(var_name, j, env) != NULL
				&& (prv_type == APPEND || prv_type == RED_IN || prv_type == RED_OUT)))
					if (ft_var_update(i, var, get_var_from_env(var_name, j, env), &var[0][i + j + 1]) == -1)
						return (-1);
		}
		else if ((var[0][i] == '$' && (is_valid_var(var[0][i + 1]) != 1))
			|| var[0][i] != '$')
			i++;
	}
	return (0);
}

int	expanden(t_token **token, char **env)
{
	t_token *tmp;
	t_type	prv_type;
	int i;

	tmp = *token;
	prv_type = tmp->type;
	while (tmp)
	{
		if (prv_type != HER_DOC)
			i = ft_expand_variable(&tmp->content, env, prv_type, 0);
		// tmp->content = expand_exit_status();
		if (i == -1)
		{
			ft_putstr_fd("minishill: malloc Error\n", 2);
			return (-1);
		}
		prv_type = tmp->type;
		tmp = tmp->next;
	}
	return (0);
}

t_type ft_get_type(char *token)
{
	if (token[0] == '|' && token[1] == '\0')
		return (PIPE);
	else if (token[0] == '>' && token[1] == '>' && token[2] == '\0')
		return (APPEND);
	else if (token[0] == '<' && token[1] == '<' && token[2] == '\0')
		return (HER_DOC);
	else if (token[0] == '>' && token[1] == '\0')
		return (RED_OUT);
	else if (token[0] == '<' && token[1] == '\0')
		return (RED_IN);
	else
		return (WORD);
}

t_token	*ft_lstlast(t_token	*lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
	{
		lst = lst->next;
	}
	return (lst);
}

void	ft_lstadd_back(t_token **lst, t_token *new)
{
	t_token	*last;

	if (!lst || !new)
		return ;
	last = ft_lstlast(*lst);
	if (last != NULL)
		last->next = new;
	else
		*lst = new;
}

t_token	*ft_list_new(char *token)
{
	t_token	*data;

	data = malloc(sizeof(t_token));
	if (!data)
		return (write(2 , "Error\n can't malloc for new_data\n", 34), NULL);
	data->content = token;
	data->type = ft_get_type(token);
	data->next = NULL;
	return (data);
}

char	*ft_add_char(char *s, int index, char to_add)
{
	int		i;
	int		j;
	char	*str;

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

char	*ft_handl_appand_herdoc(char *line, int i)
{
	if ((line[i - 1] != ' ' && line[i - 1] != '\t')
		&& (line[i + 2] != ' ' && line[i + 2] != '\t'))
	{
		line = ft_add_char(line, i, ' ');
		line = ft_add_char(line, i + 3, ' ');
	}
	else if (line[i - 1] != ' ' && line[i - 1] != '\t')
	{
		line = ft_add_char(line, i, ' ');
	}
	else if (line[i + 2] != ' ' && line[i + 2] != '\t')
	{
		line = ft_add_char(line, i + 2, ' ');
	}
	return (line);
}

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
//     if (!line) return NULL;

//     size_t len = strlen(line);
//     size_t new_len = len * 2 + 1;  // Double the size to be safe
//     char *new_line = malloc(new_len);
//     if (!new_line) return NULL;

//     size_t i = 0, j = 0;
//     while (i < len && j < new_len - 1)
//     {
//         if (line[i] == '"' || line[i] == '\'')
//         {
//             size_t quote_end = ft_skipe_qoute(line, i);
//             while (i <= quote_end && i < len && j < new_len - 1)
//                 new_line[j++] = line[i++];
//             i--; // Compensate for the i++ at the end of the loop
//         }
//         else if (i + 1 < len && 
//                  ((line[i] == '>' && line[i + 1] == '>') || 
//                   (line[i] == '<' && line[i + 1] == '<')))
//         {
//             if (i > 0 && line[i - 1] != ' ' && j < new_len - 1)
//                 new_line[j++] = ' ';
//             if (j < new_len - 1) new_line[j++] = line[i++];
//             if (j < new_len - 1) new_line[j++] = line[i];
//             if (i + 1 < len && line[i + 1] != ' ' && j < new_len - 1)
//                 new_line[j++] = ' ';
//         }
//         else if (is_spc_opr(line[i]))
//         {
//             if (i > 0 && line[i - 1] != ' ' && j < new_len - 1)
//                 new_line[j++] = ' ';
//             if (j < new_len - 1) new_line[j++] = line[i];
//             if (i + 1 < len && line[i + 1] != ' ' && j < new_len - 1)
//                 new_line[j++] = ' ';
//         }
//         else
//         {
//             if (j < new_len - 1) new_line[j++] = line[i];
//         }
//         i++;
//     }
//     new_line[j] = '\0';

//     free(line);
//     return new_line;
// }

char	*ft_add_space(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '"' || line[i] == '\'')
			i = ft_skipe_qoute(line, i);
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

int	parsing(char *line, t_token	**token, char **env)
{
	t_token *tmp;

	if (line[0] == '\0')
		return (0);
	line = ft_add_space(line);
	if (ft_cheack_qoutes(line) == -1)
		return (-1);
	line = ft_compress_spaces(line);
	get_token(token, line);
	if (*token == NULL)
		return (-1);
	tmp = *token;
	if (ft_cheack_error(*token) == 1)
		return (-1);
	if (expanden(token, env) == -1)
		return (-1);
	while (tmp)
	{
		tmp->content = ft_remove_quotes(tmp->content);
		tmp = tmp->next;
	}
	free(line);
	return (0);
}

char	*ft_remove_char(char *s, int index)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	if (ft_strlen(s) < index || !s)
		return (NULL);
	str = malloc(ft_strlen(s));
	if (!str)
		return (printf("Error :remove_char can't alloc\n"), free(s), NULL);
	while (s[i] != '\0')
	{
		if (i == index)
			i++;
		else
			str[j++] = s[i++];
	}
	str[j] = '\0';
	free(s);
	return (str);
}

char	*ft_handle_quotes(char *s, int *i)
{
	int	j;

	j = *i;
	if (s[*i] == '"')
	{
		*i = *i + 1;
		while (s[*i] != '"' && s[*i] != '\0')
			*i = *i + 1;
	}
	else if (s[*i] == '\'')
	{
		*i = *i + 1;
		while (s[*i] != '\'' && s[*i] != '\0')
			*i = *i + 1;
	}
	if ((s[j] == '"' && s[*i] == '"') || (s[j] == '\''
			&& s[*i] == '\''))
	{
		s = ft_remove_char(s, j);
		s = ft_remove_char(s, *i - 1);
		*i = *i - 1;
		return (s);
	}
	else
		return (NULL);
}

char	*ft_remove_quotes(char *s)
{
	int i;

	i = 0;
	while (s[i] != '\0')
	{
		i = ft_skipe_spaces(s, i);
		if (s[i] == '"' || s[i] == '\'')
		{
			if ((s[i] == '"' && s[i + 1] == '"') || (s[i] == '\''
					&& s[i + 1] == '\''))
			{
				s = ft_remove_char(s, i);
				s = ft_remove_char(s, i);
			}
			else
				s = ft_handle_quotes(s, &i);
		}
		else
		{
			while (s[i] != ' ' && s[i] != '\t' && s[i] != '\0'
				&& s[i] != '"' && s[i] != '\'')
				i++;
		}
	}
	return (s);
}

char	*ft_remove_char(char *s, int index)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	if (ft_strlen(s) < index || !s)
		return (NULL);
	str = malloc(ft_strlen(s));
	if (!str)
		return (printf("Error :remove_char can't alloc\n"), free(s), NULL);
	while (s[i] != '\0')
	{
		if (i == index)
			i++;
		else
			str[j++] = s[i++];
	}
	str[j] = '\0';
	free(s);
	return (str);
}

char	*ft_handle_quotes(char *s, int *i)
{
	int	j;

	j = *i;
	if (s[*i] == '"')
	{
		*i = *i + 1;
		while (s[*i] != '"' && s[*i] != '\0')
			*i = *i + 1;
	}
	else if (s[*i] == '\'')
	{
		*i = *i + 1;
		while (s[*i] != '\'' && s[*i] != '\0')
			*i = *i + 1;
	}
	if ((s[j] == '"' && s[*i] == '"') || (s[j] == '\''
			&& s[*i] == '\''))
	{
		s = ft_remove_char(s, j);
		s = ft_remove_char(s, *i - 1);
		*i = *i - 1;
		return (s);
	}
	else
		return (NULL);
}

char	*ft_remove_quotes(char *s)
{
	int i;

	i = 0;
	while (s[i] != '\0')
	{
		i = ft_skipe_spaces(s, i);
		if (s[i] == '"' || s[i] == '\'')
		{
			if ((s[i] == '"' && s[i + 1] == '"') || (s[i] == '\''
					&& s[i + 1] == '\''))
			{
				s = ft_remove_char(s, i);
				s = ft_remove_char(s, i);
			}
			else
				s = ft_handle_quotes(s, &i);
		}
		else
		{
			while (s[i] != ' ' && s[i] != '\t' && s[i] != '\0'
				&& s[i] != '"' && s[i] != '\'')
				i++;
		}
	}
	return (s);
}

static int	ft_handle_quotes(char *s, int i)
{
	int	j;

	j = i;
	if (s[i] == '"')
	{
		i++;
		while (s[i] != '"' && s[i] != '\0')
			i++;
	}
	else if (s[i] == '\'')
	{
		i++;
		while (s[i] != '\'' && s[i] != '\0')
			i++;
	}
	if ((s[j] == '"' && s[i] == '"') || (s[j] == '\''
			&& s[i] == '\''))
		return (i + 1);
	else
		return (-1);
}

int	ft_cheack_qoutes(char	*line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			i = ft_handle_quotes(line, i);
			if (i == -1)
				return (printf("Error: Unmatched quotet\n"), -1);
		}
		else
			i++;
	}
	return (1);
}

char	*ft_compress_spaces(char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == '"' || s[i] == '\'')
			i = ft_skipe_qoute(s, i) + 1;
		else if ((s[i] == ' ' && s[i + 1] == ' ') || (s[i] == '\t'
				&& s[i + 1] == '\t'))
			s = ft_remove_char(s, i);
		else
			i++;
	}
	return (s);
}

int	get_token_len(char	*s, int i)
{
	int	j;

	j = i;
	if ((s[i] == '"' && s[i + 1] == '"') || (s[i] == '\''
			&& s[i + 1] == '\''))
		return (0);
	else if (s[i] == '"' || s[i] == '\'')
		i = ft_skipe_qoute(s, i) + 1;
	else
	{
		while (s[i] != '\0' && s[i] != ' ' && s[i] != '\t')
			i++;
	}
	return (i - j);
}

void	get_token(t_token **token, char	*s)
{
	int		i;
	int		j;
	t_token	*new;

	i = 0;
	while (s[i] != '\0')
	{
		i = ft_skipe_spaces(s, i);
		if (s[i] == '\0')
			break ;
		j = get_token_len(s, i);
		if (j == 0)
			i += 2;
		else
		{
			new = ft_list_new(ft_substr(s, i, j));
			// if (new->content[0] == '\"')
			// 	printf ("|%s->|%c||\n", new->content, new->content[i + j]);
			ft_lstadd_back(token, new);
		}
		i = i + j;
	}
}

void	ft_putstr_fd(char *str, int fd)
{
	if (!str)
		write (fd, "(null)\n", 7);
	else
		write (fd, str, ft_strlen(str));
}

int is_valid_var(char c)
{
	if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z')
	|| (c >= 'A' && c <= 'Z') || c == '_')
		return (1);
	else
		return (0);
}

char	*ft_strdup(char *s1)
{
	char	*s;
	int		i;

	i = 0;
	s = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (s == NULL)
		return (NULL);
	while (s1[i])
	{
		s[i] = s1[i];
		i++;
	}
	s[i] = '\0';
	return (s);
}

char	*ft_strndup(char *str, int n)
{
	char	*res;
	int		i;

	i = 0;
	res = malloc(sizeof(char) * (n + 1));
	if (!res)
		return (NULL);
	while (str[i] != '\0' && i < n)
	{
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char	*ft_strjoin(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*s;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1)
		return (ft_strdup(s2));
	else if (!s2)
		return (ft_strdup(s1));
	s = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!s)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		s[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
		s[i++] = s2[j++];
	s[i] = '\0';
	free(s1);
	return (s);
}