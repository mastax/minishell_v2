/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <elel-bah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 15:24:05 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/08/31 13:09:58 by elel-bah         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../mini_shell.h"

char *get_env_value(t_env *env, const char *key) //helper cd
{
	int key_len;
	int i;

	key_len = ft_strlen(key);
	i = 0;
	while (i < env->count)
	{
		if (ft_strncmp(env->env_vars[i], key, key_len) == 0 && env->env_vars[i][key_len] == '=')
			return env->env_vars[i] + key_len + 1;
		i++;
	}
	return NULL;
}

int change_to_home(t_env *env)
{
	char *home;

	home = get_env_value(env, "HOME");
	if (!home)
	{
		ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
		return (1);
	}
	return (chdir(home));
}

int change_to_previous(t_env *env)
{
	char *oldpwd;

	oldpwd = get_env_value(env, "OLDPWD");
	if (!oldpwd)
	{
		ft_putstr_fd("cd: OLDPWD not set\n", STDERR_FILENO);
		return (1);
	}
	ft_putendl_fd(oldpwd, STDOUT_FILENO);
	return (chdir(oldpwd));
}

t_env *allocate_env(int count)
{
	t_env *env;

	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->env_vars = malloc(sizeof(char *) * (count + 1));
	if (!env->env_vars)
	{
		free(env);
		return (NULL);
	}
	env->count = count;
	return (env);
}

t_env *create_env(char **envp)
{
	t_env *env;
	int count;
	int i;

	count = 0;
	while (envp[count])
		count++;
	if (count == 0)
		return (create_env_v_i(5));
	env = allocate_env(count);
	if (!env)
		return (NULL);
	i = 0;
	while (i < count && (env->env_vars[i] = ft_strdup(envp[i])))
		i++;
	if (i < count)
	{
		while (--i >= 0)
			free(env->env_vars[i]);
		free(env->env_vars);
		free(env);
		return (NULL);
	}
	env->env_vars[count] = NULL;
	return (env);
}
