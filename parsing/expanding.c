/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <elel-bah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 09:55:47 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/08/31 17:55:56 by elel-bah         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../mini_shell.h"

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

int	expanding_helper(char *s, int i)
{
	if (s[i] == '\'')
	{
		i++;
		while (s[i] != '\0' && s[i] != '\'')
			i++;
		if (s[i] == '\'')
			i++;
		return (i);
	}
	return (0);
}

int	check_can_expand(char *var_name, t_env *env, t_type prv_type, t_token *t)
{
	if (prv_type == WORD || prv_type == PIPE)
	{
		if (t->content[0] != '"' && t->content[0] != '\'')
			t->qout_rm = false;
		return (1);
	}
	else if ((get_var_from_env(var_name, t->j, env) != NULL)
		&& (prv_type == APPEND || prv_type == RED_IN || prv_type == RED_OUT))
	{
		if (t->content[0] != '"' && t->content[0] != '\'')
			t->qout_rm = false;
		return (1);
	}
	else if ((get_var_from_env(var_name, t->j, env) == NULL)
		&& (prv_type == APPEND || prv_type == RED_IN || prv_type == RED_OUT))
	{
		if (t->content[ft_strlen(t->content) - 1] == '\''
			|| t->content[ft_strlen(t->content) - 1] == '"')
			return (1);
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(t->content, 2);
		ft_putstr_fd(": ambiguous redirect\n", 2);
		return (0);
	}
	return (0);
}

int	ft_expand_variable(t_token *t, t_env *env, t_type prv_type, int i)
{
	char	*var_name;

	while (t->content[i] != '\0')
	{
		if (t->content[i] == '\'' && check_induble(t->content, i) == 0)
			i = expanding_helper(t->content, i);
		else if (t->content[i] == '$' && (is_valid_var(t->content[i + 1]) == 1))
		{
			t->j = ft_name_len(t->content, i + 1);
			var_name = ft_substr(t->content, i + 1, t->j);
			if (!var_name)
				return (ft_putstr_fd("minishell : malloc error", 2), -1);
			if (check_can_expand(var_name, env, prv_type, t) == 0)
				return (free(var_name), -1);
			if (check_can_expand(var_name, env, prv_type, t) == 1)
				if (ft_var_update(i, &(t->content), get_var_from_env(var_name,
							t->j, env), &t->content[i + t->j + 1]) == -1)
					return (ft_putstr_fd("minishell : malloc error", 2), -1);
		}
		else if ((t->content[i] == '$' && (is_valid_var(t->content[i + 1])
					!= 1)) || (t->content[i] != '$' && t->content[i] != '\0'))
			i++;
	}
	return (0);
}

int	expanding(t_token **token, t_env *env, int exit_status, t_type prv_type)
{
	t_token	*t;

	t = *token;
	while (t)
	{
		if (prv_type != HER_DOC)
		{
			if (ft_expand_variable(t, env, prv_type, 0) == -1)
				return (-1);
			if (ft_expand_exit_status(&t->content, exit_status, 0) == -1)
				return (ft_putstr_fd("minishell : malloc error", 2), -1);
		}
		if (ft_have_sp_tb(t->content) == 1)
		{
			if (ft_handl_spichel_cond(token, t, t->next, &prv_type) == -1)
				return (-1);
			t = *token;
		}
		else
		{
			prv_type = t->type;
			t = t->next;
		}
	}
	return (0);
}
