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

// int	ft_strncmp(char *s1, const char *s2, size_t n)
// {
// 	unsigned int	i;

// 	i = 0;
// 	while (s1[i] == s2[i] && s1[i] != '\0' && i < n)
// 		i++;
// 	if (i == n)
// 		return (0);
// 	else
// 		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
// }

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