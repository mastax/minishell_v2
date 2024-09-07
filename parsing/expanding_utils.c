/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <elel-bah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 10:00:52 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/08/31 17:57:42 by elel-bah         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../mini_shell.h"

int	ft_name_len(char *var, int i)
{
	int	len;

	len = 0;
	if (var[i] > '0' && var[i] <= '9')
		return (1);
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

void	ambiguous_error(char *arg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": ambiguous redirect\n", 2);
}

int	ft_handl_spichel_cond(t_token **token, t_token *now, t_token *next_token
, t_type *prv_type)
{
	t_token	*new_tokens;
	t_token	*tmp;

	new_tokens = NULL;
	if (*prv_type == APPEND || *prv_type == RED_IN || *prv_type == RED_OUT)
		return (ambiguous_error(now->content), -1);
	*prv_type = WORD;
	if (get_token(&new_tokens, now->content, 2) == -1)
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
