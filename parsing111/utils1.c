/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:43:46 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/08/14 12:10:12 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

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

int	ft_check_qoutes(char	*line)
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
			i = ft_skipe_qoute(s, i);
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
	// printf("1%c--%d\n", s[i], i);
	if (s[i] == '"' || s[i] == '\'')
		i = ft_skipe_qoute(s, i);
	else
	{
		while (s[i] != '\0' && s[i] != ' ' && s[i] != '\t'
			&& s[i] != '"' && s[i] != '\'')
			i++;
		if (s[i] == '"' || s[i] == '\'')
			i = ft_skipe_qoute(s, i);
	}
	// printf("2%c--%d\n", s[i], i);
	return (i - j);
}

int	get_token(t_token **token, char	*s, int z)
{
	int		i;
	int		j;
	t_token	*new;
	char	*contant;

	i = 0;
	while (s[i] != '\0')
	{
		i = ft_skipe_spaces(s, i);
		if (s[i] == '\0')
			break ;
		j = get_token_len(s, i);
		contant = ft_substr(s, i, j);
		if (!contant)
			return (-1);
		// printf("%s\n", contant);
		new = ft_list_new(contant, z);
		if (!new)
			return (-1);
		ft_lstadd_back(token, new);
		i = i + j;
	}
	return (0);
}
