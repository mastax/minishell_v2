#include "../mini_shell.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t			total_size;
	void			*ptr;

	if ((int )count < 0 || (int)size < 0)
		return (NULL);
	total_size = count * size;
	ptr = malloc(total_size);
	if (ptr == NULL)
		return (NULL);
	ft_memset(ptr, 0, total_size);
	return (ptr);
}

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	remai_len;
	size_t	i;

	i = 0;
	src_len = ft_strlen(src);
	if (!dstsize && !dst)
	{
		return (src_len);
	}
	dst_len = ft_strlen(dst);
	remai_len = dstsize - dst_len;
	if (dst_len >= dstsize)
	{
		return (src_len + dstsize);
	}
	while (src[i] && i < (remai_len - 1))
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return (dst_len + src_len);
}

// void	ft_putstr_fd(char *s, int fd)
// {
// 	if (!s || !fd)
// 		return ;
// 	else
// 		write(fd, s, ft_strlen(s));
// }

int	is_number(int c)
{
	if (c >= '0' && c <= '9')
	{
		return (1);
	}
	return (0);
}
