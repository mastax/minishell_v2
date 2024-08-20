/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:53:21 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/08/13 09:53:47 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

char	*ft_remove_char(char *s, unsigned int index)
{
	unsigned int	i;
	int				j;
	char			*str;

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
	int	i;

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
