/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <elel-bah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 15:23:55 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/08/29 17:04:50 by elel-bah         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../mini_shell.h"

t_env *populate_env_vars(t_env *env, const char **vars)
{
	int	i;
	int	j;
	char current_dir[PATH_MAX];

	i = 0;
	j = 0;
	if (getcwd(current_dir, sizeof(current_dir)) == NULL)
		return (NULL);
	while (i < env->count)
	{
		if (i == 1)
			env->env_vars[i] = ft_strjoin(ft_strdup(vars[i]), current_dir);
		else
			env->env_vars[i] = ft_strdup(vars[i]);
		if (!env->env_vars[i])
		{
			while (j < i)
				free(env->env_vars[j++]);
			return (free(env->env_vars), free(env), NULL);
		}
		i++;
	}
	env->env_vars[env->count] = NULL;
	return (env);
}

t_env *create_env_v_i(int count)
{
	t_env *env;
	const char *init_vars[6];

    init_vars[0] = "PATH=/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.";
    init_vars[1] = "PWD=";
    init_vars[2] = "OLDPWD";
    init_vars[3] = "SHLVL=1";
    init_vars[4] = "_=/usr/bin/env";
    init_vars[5] = NULL;
	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->env_vars = malloc(sizeof(char *) * (count + 1));
	if (!env->env_vars)
		return (free(env), NULL);
	env->count = count;
	return (populate_env_vars(env, init_vars));
}
