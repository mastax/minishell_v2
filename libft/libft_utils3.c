#include "../mini_shell.h"

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

int		ft_is_space(int k)
{
	k = (unsigned char)k;
	if (k == '\t' || k == '\n' || k == '\v' || k == '\f' || k == '\r'
		|| k == ' ')
		return (1);
	return (0);
}