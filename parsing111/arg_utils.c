/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-hasn <sel-hasn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 10:03:28 by sel-hasn          #+#    #+#             */
/*   Updated: 2024/08/13 10:03:43 by sel-hasn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

t_arg	*ft_arglast(t_arg	*lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
	{
		lst = lst->next;
	}
	return (lst);
}

void	ft_argadd_back(t_arg **lst, t_arg *new)
{
	t_arg	*last;

	if (!lst || !new)
		return ;
	last = ft_arglast(*lst);
	if (last != NULL)
		last->next = new;
	else
		*lst = new;
}

t_arg	*ft_arg_new(int fd)
{
	t_arg	*arg;

	arg = malloc(sizeof(t_arg));
	if (!arg)
		return (ft_putstr_fd("Error\n can't malloc for new_arg\n", fd), NULL);
	arg->arg = NULL;
	arg->red = NULL;
	arg->next = NULL;
	return (arg);
}
