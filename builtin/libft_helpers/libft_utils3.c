#include "../../mini_shell.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t			src_len;
	size_t			i;

	src_len = 0;
	while (src[src_len] != '\0')
		src_len++;
	if (dstsize > 0)
	{
		i = 0;
		while (src[i] != '\0' && i < dstsize - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (src_len);
}

// char	*ft_strjoin(char const *s1, char const *s2)
// {
// 	size_t	lenght1;
// 	size_t	lenght2;
// 	size_t	total_lenght;
// 	char	*result;

// 	lenght1 = 0;
// 	lenght2 = 0;
// 	if (s1 != NULL)
// 		lenght1 = ft_strlen(s1);
// 	if (s2 != NULL)
// 		lenght2 = ft_strlen(s2);
// 	total_lenght = lenght1 + lenght2;
// 	result = (char *)malloc((total_lenght + 1) * sizeof(char));
// 	if (result == NULL)
// 		return (NULL);
// 	if (s1 != NULL)
// 		ft_memcpy(result, s1, lenght1);
// 	if (s2 != NULL)
// 		ft_memcpy(result + lenght1, s2, lenght2);
// 	result[total_lenght] = '\0';
// 	return (result);
// }

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	x;

	if (dst == src)
		return (dst);
	x = 0;
	while (x < n)
	{
		((unsigned char *)dst)[x] = ((unsigned char *)src)[x];
		x++;
	}
	return (dst);
}

int	ft_isalnum(int c)
{
	if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}