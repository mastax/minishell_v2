/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 14:50:12 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/08/25 22:41:42 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

char	*ft_get_path(char *cmd, int *exit_status)
{
	char		*path;
	struct stat	statfile;

	if (stat(cmd, &statfile) == 0 && S_ISDIR(statfile.st_mode)
		&& ft_strchr(cmd, '/') != NULL)
	{
		*exit_status = 126;
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": is a directory\n", 2);
		exit (*exit_status);
	}
	path = cmd;
	if (ft_strchr(cmd, '/') != NULL && access(cmd, X_OK) == -1)
	{
		*exit_status = 127;
		if (access(cmd, F_OK) == 0)
			*exit_status = 126;
		perror(cmd);
		exit (*exit_status);
	}
	if (ft_strchr(cmd, '/') != NULL && access(cmd, X_OK) == 0)
		return (path);
	return (NULL);
}
