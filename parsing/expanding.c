/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:55:47 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/08/14 12:11:25 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

int	ft_strncmp(char *s1, const char *s2, size_t n)
{
	size_t			i;
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

char	*get_var_from_env(char *var, int var_len, t_env *env)
{
	int	i;

	i = 0;
	while (env->env_vars[i] != NULL)
	{
		if (ft_strncmp(env->env_vars[i], var, var_len) == 0
			&& env->env_vars[i][var_len] == '=')
			return (&(env->env_vars[i][var_len + 1]));
		i++;
	}
	return (NULL);
}

int	ft_var_update(int i, char **var, char *to_update, char	*secend_part)
{
	char	*first_part;
	char	*update_var;

	first_part = NULL;
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

int	ft_expand_variable(char **var, t_env *env, t_type prv_type, int i)
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
			if (prv_type == WORD || (get_var_from_env(var_name, j, env) != NULL
					&& (prv_type == APPEND || prv_type == RED_IN
						|| prv_type == RED_OUT)) || prv_type == PIPE)
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

int	expanding(t_token **token, t_env *env, int exit_status)
{
	t_token	*t;
	t_type	prv_type;

	t = *token;
	prv_type = t->type;
	while (t)
	{
		if (prv_type != HER_DOC)
		{
			if (ft_expand_variable(&t->content, env, prv_type, 0) == -1)
				return (ft_putstr_fd("minishell : malloc error", 2), -1);
			if (ft_expand_exit_status(&t->content, exit_status) == -1)
				return (ft_putstr_fd("minishell : malloc error", 2), -1);
		}
		prv_type = t->type;
		if (ft_have_sp_tb(t->content) == 1)
		{
			if (ft_handl_spichel_cond(token, t, t->next, t->content) == -1)
				return (-1);
			t = *token;
		}
		else
			t = t->next;
	}
	return (exit_status);
}
