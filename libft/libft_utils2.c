#include "../mini_shell.h"

static int	overflow_ft(int sign)
{
	if (sign == -1)
		return (0);
	else
		return (-1);
}

int	ft_atoi(const char *str)
{
	int				i;
	int				sign;
	unsigned long	result;
	int				counter;

	i = 0;
	sign = 1;
	result = 0;
	counter = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if ((str[i] == '-') || (str[i] == '+'))
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		counter++;
		if (result * 10 + str[i] - '0' > LONG_MAX && counter >= 20)
			return (overflow_ft(sign));
		result = result * 10 + str[i++] - '0';
	}
	return (result * sign);
}

// char	*ft_strdup(const char *src)
// {
// 	int		i;
// 	char	*dst;

// 	i = 0;
// 	dst = (char *)malloc(sizeof(char) * ft_strlen(src) + 1);
// 	if (!dst)
// 		return (NULL);
// 	while (src[i])
// 	{
// 		dst[i] = src[i];
// 		i++;
// 	}
// 	dst[i] = '\0';
// 	return (dst);
// }

// char	*ft_substr(char *s, unsigned int start, size_t len)
// {
// 	size_t	x;
// 	size_t	y;
// 	char	*substring;

// 	if (!s)
// 		return (NULL);
// 	x = 0;
// 	while (x < len && (start < ft_strlen(s)) && s[start + x])
// 		x++;
// 	y = 0;
// 	substring = malloc((x + 1) * sizeof(char));
// 	if (!substring)
// 		return (NULL);
// 	while (y < x && start < ft_strlen(s))
// 	{
// 		substring[y] = s[start + y];
// 		y++;
// 	}
// 	substring[x] = '\0';
// 	return (substring);
// }

int ft_strcmp(const char *s1, const char *s2)
{
  // Iterate over characters until null terminator or mismatch is found
  while (*s1 != '\0' && *s1 == *s2)
  {
	s1++;
    s2++;
  }
  // Return the difference between the characters where mismatch occurred
  return (*s1 - *s2);
}