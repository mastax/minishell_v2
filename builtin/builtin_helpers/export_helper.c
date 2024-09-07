/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <elel-bah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 11:05:22 by elel-bah          #+#    #+#             */
/*   Updated: 2024/08/29 11:15:20 by elel-bah         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../mini_shell.h"

static char	**copy_env_vars(t_env *env)
{
	char	**sorted_env;
	int		i;

	sorted_env = malloc(sizeof(char *) * (env->count + 1));
	if (!sorted_env)
		return (NULL);
	i = 0;
	while (i < env->count)
	{
		sorted_env[i] = ft_strdup(env->env_vars[i]);
		i++;
	}
	sorted_env[i] = NULL;
	return (sorted_env);
}

static void	sort_env_vars(char **sorted_env, int count)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(sorted_env[j], sorted_env[j + 1]) > 0)
			{
				temp = sorted_env[j];
				sorted_env[j] = sorted_env[j + 1];
				sorted_env[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

static void	print_env_vars(char **sorted_env, int count)
{
	int     i;
	char    *name;
	char    *value;

	i = 0;
	while (i < count)
	{
		ft_putstr_fd("declare -x ", 1);
		name = sorted_env[i];
		value = ft_strchr(sorted_env[i], '=');
		if (value)
		{
			*value = '\0';
			ft_putstr_fd(name, 1);
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(value + 1, 1);
			ft_putendl_fd("\"", 1);
			*value = '=';
		}
		else
			ft_putendl_fd(sorted_env[i], 1);
		i++;
	}
}

static void	free_sorted_env(char **sorted_env, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(sorted_env[i]);
		i++;
	}
	free(sorted_env);
}

void	print_sorted_env(t_env *env)
{
	char**sorted_env;

	sorted_env = copy_env_vars(env);
	if (!sorted_env)
		return;
	sort_env_vars(sorted_env, env->count);
	print_env_vars(sorted_env, env->count);
	free_sorted_env(sorted_env, env->count);
}
