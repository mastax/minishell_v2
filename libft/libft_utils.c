/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <elel-bah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:56:39 by elel-bah          #+#    #+#             */
/*   Updated: 2024/08/31 17:47:52 by elel-bah         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../mini_shell.h"

void	ft_putendl_fd(char *s, int fd)
{
	if (!s)
		return ;
	else
	{
		write(fd, s, ft_strlen(s));
		write(fd, "\n", 1);
	}
}

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*p_b;

	p_b = b;
	while (len-- > 0)
	{
		*p_b++ = c;
	}
	return (b);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s != '\0')
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}

size_t ft_strlen(const char *s)
{
    int i;

    i = 0;
	if (!s)
		return (0);
    while(s[i])
        i++;
    return(i);
}
char	*ft_concat(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*s;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1)
		return (ft_strdup(s2));
	else if (!s2)
		return (ft_strdup(s1));
	s = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!s)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		s[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
		s[i++] = s2[j++];
	s[i] = '\0';
	// free(s1);
	return (s);
}