/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 10:00:52 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/08/13 10:01:34 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

int	ft_name_len(char *var, int i)
{
	int	len;

	len = 0;
	while (var[i + len] != '\0' && (is_valid_var(var[i + len]) == 1))
		len++;
	return (len);
}

int	ft_have_sp_tb(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	i = ft_skipe_spaces(s, i);
	if (s[i] == '\0')
		return (0);
	i = 0;
	if (s[0] == '|' && s[1] == '\0')
		return (0);
	if (s[0] == '"' || s[0] == '\'')
		return (0);
	while (s[i])
	{
		if (s[i] == '=' && (s[i + 1] == '\'' || s[i + 1] == '"'))
			return (0);
		if (s[i] == ' ' || s[i] == '\t')
			return (1);
		i++;
	}
	return (0);
}

t_token	*list_befor_last(t_token *list)
{
	t_token	*tmp;

	tmp = list;
	if (!tmp)
		return (NULL);
	while (tmp->next->next)
		tmp = tmp->next;
	return (tmp);
}

int	ft_handl_spichel_cond(t_token **token, t_token *now, t_token *next_token,
char *content)
{
	t_token	*new_tokens;
	t_token	*tmp;

	new_tokens = NULL;
	if (get_token(&new_tokens, content, 2) == -1)
		return (-1);
	tmp = *token;
	if (tmp == now)
		*token = new_tokens;
	else
	{
		while (tmp->next != now)
			tmp = tmp->next;
		tmp->next = new_tokens;
	}
	tmp = new_tokens;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = next_token;
	free(now->content);
	free(now);
	return (0);
}
