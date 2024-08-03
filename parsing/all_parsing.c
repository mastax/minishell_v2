#include "mini_shell.h"


char	*ft_handle_quotes(char *s, int *i);
int	check_pipe(t_token *token, t_token *next_token)
{
	if (token->type == PIPE && next_token->type == PIPE)
		return (1);
	else
		return (0);
}

int	check_redirect(t_token *token, t_token *next_token)
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

int	ft_check_error(t_token *token)
{
	if (token->type == PIPE)
		return (printf("syntax error near unexpected token '%s'\n"
					, token->content), 1);
	while (token->next)
	{
		if (check_pipe(token, token->next) != 0
			|| check_redirect(token, token->next) != 0)
			return (printf("syntax error near unexpected token '%s'\n"
					, token->content), 1);
		token = token->next;
	}
	// if (token->type != WORD)
	// 	return (printf("syntax error near unexpected token 'newline'\n"
	// 			, token->content), 1);
	return (0);
}

// int	ft_strncmp(char *s1, char *s2, size_t n)
// {
// 	size_t	i;
// 	unsigned char	*str1;
// 	unsigned char	*str2;

// 	str1 = (unsigned char *)s1;
// 	str2 = (unsigned char *)s2;
// 	i = 0;
// 	while (i < n && (str1[i] != '\0' && str2[i] != '\0'))
// 	{
// 		if (str1[i] != str2[i])
// 			return (str1[i] - str2[i]);
// 		i++;
// 	}
// 	return (0);
// }

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
		if (ft_strncmp(env[i], var, var_len) == 0)
			return (&env[i][var_len + 1]);
		i++;
	}
	return (NULL);
}

char	*ft_expand_variable(char *var, char **env)
{
	int	i;
	int	j;
	char	*var_name;

	i = 0;
	while (var[i])
	{
		if (var[i] == '\'')
		{
			i++;
			while (var[i] != '\'' && var[i] != '\0')
				i++;
		}
		else if (var[i] == '$' && (is_valid_var(var[i + 1]) == 1))
		{
			j = ft_name_len(var, i + 1);
			var_name = ft_substr(var, i + 1, j);
			if (!var_name)
				return (NULL);
			printf("hello form expand var : %s len is : %d %c\n", var_name, j, var[i + j]);
			printf("expand val : %s\n",get_var_from_env(var_name, j, env));
			// var = ft_handl_var(i - 1, i + j, var, var_name);
		}
		i++;
	}
	return (var);
}

int	expanden(t_token **token, char **env)
{
	t_token *tmp;

	tmp = *token;
	while (tmp)
	{
		tmp->content = ft_expand_variable(tmp->content, env);
		// tmp->content = expand_exit_status();
		if (!tmp->content)
		{
			ft_putstr_fd("minishill: malloc Error\n", 2);
				return (-1);
		}
		tmp = tmp->next;
	}
	return (0);
}

char	*ft_add_char(char *s, int index, char to_add)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	if (ft_strlen(s) < (unsigned int)index || !s) // cast index
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

// static char	*print_type(t_type type)
// {
// 	if (WORD == type)
// 		return ("WORD");
// 	else if (PIPE == type)
// 		return ("PIPE");
// 	else if (RED_IN == type)
// 		return ("RED_IN");
// 	else if (RED_OUT == type)
// 		return ("RED_OUT");
// 	else if (HER_DOC == type)
// 		return ("HER_DOC");
// 	else if (APPEND == type)
// 		return ("APPEND");
// 	else
// 		return (NULL);
// }


t_arg *convert_tokens_to_args(t_token *tokens)
{
    t_arg *head = NULL;
    t_arg *current = NULL;
    t_token *token = tokens;

    while (token)
    {
        if (token->type == PIPE)
        {
            if (current)
                current->next = malloc(sizeof(t_arg));
            else
                head = malloc(sizeof(t_arg));
            current = (current) ? current->next : head;
            current->arg = NULL;
            current->red = NULL;
            current->next = NULL;
        }
        else
        {
            if (!current)
            {
                head = malloc(sizeof(t_arg));
                current = head;
                current->arg = NULL;
                current->red = NULL;
                current->next = NULL;
            }
            
            if (token->type == WORD)
            {
                // Add to arg
                int arg_count = 0;
                while (current->arg && current->arg[arg_count]) arg_count++;
                current->arg = realloc(current->arg, sizeof(char*) * (arg_count + 2));
                current->arg[arg_count] = strdup(token->content);
                current->arg[arg_count + 1] = NULL;
            }
            else if (token->type == RED_IN || token->type == RED_OUT || token->type == HER_DOC || token->type == APPEND)
            {
                // Add to red
                int red_count = 0;
                while (current->red && current->red[red_count]) red_count++;
                current->red = realloc(current->red, sizeof(char*) * (red_count + 3));
                current->red[red_count] = strdup(token->content);
                token = token->next; // Move to the next token for the filename
                if (token)
                {
                    current->red[red_count + 1] = strdup(token->content);
                    current->red[red_count + 2] = NULL;
                }
            }
        }
        token = token->next;
    }
    return head;
}


t_arg *parsing(char *line, char **env)
{
    t_token *token = NULL;
    t_arg *args = NULL;

    // printf("Debug: Entering parsing function\n");

    if (line[0] == '\0')
    {
        // printf("Debug: Empty line, returning NULL\n");
        return NULL;
    }

    // Add spaces around special operators
    line = ft_add_space(line);
    if (!line)
    {
        printf("Debug: ft_add_space failed, returning NULL\n");
        return NULL;
    }

    // Check for unmatched quotes
    if (ft_check_qoutes(line) == -1)
    {
        // printf("Debug: Unmatched quotes, returning NULL\n");
        free(line);
        return NULL;
    }

    // Compress multiple spaces into single spaces
    line = ft_compress_spaces(line);
    if (!line)
    {
        // printf("Debug: ft_compress_spaces failed, returning NULL\n");
        return NULL;
    }

    // Tokenize the input
    get_token(&token, line);
    if (token == NULL)
    {
        // printf("Debug: Tokenization failed, returning NULL\n");
        free(line);
        return NULL;
    }

    // Check for syntax errors
    if (ft_check_error(token) == 1)
    {
        // printf("Debug: Syntax error detected, returning NULL\n");
        free(line);
        // Free the token list
        while (token)
        {
            t_token *next = token->next;
            free(token->content);
            free(token);
            token = next;
        }
        return NULL;
    }

    // Expand variables
    if (expanden(&token, env) == -1)
    {
        // printf("Debug: Variable expansion failed, returning NULL\n");
        free(line);
        // Free the token list
        while (token)
        {
            t_token *next = token->next;
            free(token->content);
            free(token);
            token = next;
        }
        return NULL;
    }

    // Convert tokens to t_arg structure
    args = convert_tokens_to_args(token);
    if (args == NULL)
    {
        // printf("Debug: convert_tokens_to_args failed, returning NULL\n");
    }

    // Free the token list
    while (token)
    {
        t_token *next = token->next;
        free(token->content);
        free(token);
        token = next;
    }

    free(line);
    // printf("Debug: Exiting parsing function. Returning %p\n", (void*)args);
    return args;
}

// t_arg *parsing(char *line, char **env)
// {
//     t_token *token = NULL;
//     t_arg *args = NULL;

//     if (line[0] == '\0')
//         return NULL;

//     // Add spaces around special operators
//     line = ft_add_space(line);
//     if (!line)
//         return NULL;

//     // Check for unmatched quotes
//     if (ft_cheack_qoutes(line) == -1)
//     {
//         free(line);
//         return NULL;
//     }

//     // Compress multiple spaces into single spaces
//     line = ft_compress_spaces(line);
//     if (!line)
//         return NULL;

//     // Tokenize the input
//     get_token(&token, line);
//     if (token == NULL)
//     {
//         free(line);
//         return NULL;
//     }

//     // Check for syntax errors
//     if (ft_cheack_error(token) == 1)
//     {
//         free(line);
//         // Free the token list
//         while (token)
//         {
//             t_token *next = token->next;
//             free(token->content);
//             free(token);
//             token = next;
//         }
//         return NULL;
//     }

//     // Expand variables
//     if (expanden(&token, env) == -1)
//     {
//         free(line);
//         // Free the token list
//         while (token)
//         {
//             t_token *next = token->next;
//             free(token->content);
//             free(token);
//             token = next;
//         }
//         return NULL;
//     }

//     // Convert tokens to t_arg structure
//     // args = convert_tokens_to_args(token);

//     // Free the token list
//     while (token)
//     {
//         t_token *next = token->next;
//         free(token->content);
//         free(token);
//         token = next;
//     }

//     free(line);
//     return args;
// }

// int	parsing(char *line, t_token	*token, char **env)
// {
// 	// printf("befor add space : %s\n", line);
// 	if (line[0] == '\0')
// 		return (0);
// 	line = ft_add_space(line);
// 	if (ft_cheack_qoutes(line) == -1)
// 		return (-1);
// 	line = ft_compress_spaces(line);
// 	get_token(&token, line);
// 	if (token == NULL)
// 		return (printf("no token\n"), 0);
// 	if (ft_cheack_error(token) == 1)
// 		return (-1);
// 	if (expanden(&token, env) == -1)
// 		return (-1);
// 	// printf("no token\n");
// 	while (token)
// 	{
// 		printf("1!!  Token : {%s}------->>>>>>>> Type : [%s]\n", token->content, print_type(token->type));
// 		// token->content = ft_remove_quotes(token->content, malloc(sizeof(int)));
// 		// printf("2>!  Token : {%s}------->>>>>>>> Type : [%s]\n\n", token->content, print_type(token->type));
// 		token = token->next;
// 	}
// 	printf("afther add space : {>%s<}\n", line);
// 	free(line);
// 	return (0);
// }

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
	// printf("hello");
	if (!lst)
		return (NULL);
	// printf("hello1");
	while (lst->next != NULL)
	{
		lst = lst->next;
		// printf("hello2");	
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

char	*ft_remove_char(char *s, int index)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	if (ft_strlen(s) < (unsigned int)index || !s)
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
		return (s);
	}
	else
		return (NULL);
}

char	*ft_remove_quotes(char *s, int	*i)
{
	if (!i)
		return (printf("Error: Unable to allocate memory for i\n"), NULL);
	*i = 0;
	while (s[*i] != '\0')
	{
		*i = ft_skipe_spaces(s, *i);
		if (s[*i] == '"' || s[*i] == '\'')
		{
			if ((s[*i] == '"' && s[*i + 1] == '"') || (s[*i] == '\''
					&& s[*i + 1] == '\''))
			{
				s = ft_remove_char(s, *i);
				s = ft_remove_char(s, *i);
			}
			else
				s = ft_handle_quotes(s, i);
		}
		else
		{
			while (s[*i] != ' ' && s[*i] != '\t' && s[*i] != '\0'
				&& s[*i] != '"' && s[*i] != '\'')
				*i = *i + 1;
		}
	}
	return (s);
}

int is_valid_var(char c)
{
	if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z')
	|| (c >= 'A' && c <= 'Z') || c == '_')
		return (1);
	else
		return (0);
}

static int	ft_handle_quotess(char *s, int i)
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

int	ft_check_qoutes(char	*line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			i = ft_handle_quotess(line, i);
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

int	is_spc_opr(char c)
{
	if (c == '|' || c == '>' || c == '<')
		return (1);
	return (0);
}

int	ft_skipe_spaces(char *line, int i)
{
	while ((line[i] == ' ' || line[i] == '\t') && line[i] != '\0')
		i++;
	return (i);
}

int	ft_skipe_qoute(char	*s, int i)
{
	if ((s[i] == '"' && s[i + 1] == '"') || (s[i] == '\''
			&& s[i + 1] == '\''))
		i += 1;
	else if (s[i] == '"')
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
	if (s[i + 1] != ' ' || s[i + 1] != '\t' || s[i + 1] != '\0'
		||s[i + 1] != '|' || s[i + 1] != '>' || s[i + 1] != '<')
	{
		while (s[i] != ' ' && s[i] != '\t' && s[i] != '\0'
			&& s[i + 1] != '|' && s[i + 1] != '>' && s[i + 1] != '<')
			i++;
	}
	return (i + 1);
}

// int	main(int arc, char **arv, char **env)
// {
// 	char	*line;
// 	int		exit_status;
// 	t_token	*token;

// 	token = NULL;
// 	while (1)
// 	{
// 		line = readline("-> minishell : ");
// 		exit_status = parsing(line, token, env);
// 		if (exit_status == -1)
// 			free(line);
// 		// if (!line)
// 		// {
// 		// 	printf("dsalkklsdfjladsjlfkjdlskfj");
// 		// 	break ;
// 		// }
// 			// return (free(line), 1);
// 	}
// }

