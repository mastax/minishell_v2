#include "../mini_shell.h"

void	ft_putnbr_fd(int n, int fd)
{
	if (n > 2147483647 || n < -2147483648)
		return ;
	if (n == 0)
	{
		ft_putchar_fd('0', fd);
		return ;
	}
	if (n == -2147483648)
	{
		ft_putstr_fd("-2147483648", fd);
		return ;
	}
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		n *= -1;
	}
	if (n >= 10)
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putnbr_fd(n % 10, fd);
	}
	else
		ft_putchar_fd(n + '0', fd);
}

void	ft_putchar_fd(char c, int fd)
{
	if (fd < 0)
		return ;
	write(fd, &c, 1);
}

char *ft_strcpy(char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s2[i])
	{
		s1[i] = s2[i];
		i++;
	}
	s1[i] = 0;
	return (s1);
}

static int	lenght(long nbr)
{
	int	lenght;

	lenght = 0;
	if (nbr == 0)
		return (1);
	if (nbr < 0)
	{
		nbr *= -1;
		lenght++;
	}
	while (nbr > 0)
	{
		nbr /= 10;
		lenght++;
	}
	return (lenght);
}

char	*ft_itoa(int n)
{
	char	*str;
	long	nbr;
	int		i;

	nbr = n;
	i = lenght(nbr);
	str = (char *)malloc (i + 1);
	if (!str)
		return (NULL);
	str[i] = '\0';
	if (nbr == 0)
		str[0] = 48;
	else if (n < 0)
	{
		str[0] = '-';
		nbr = -nbr;
	}
	while (nbr > 0 && i-- >= 0)
	{
		str[i] = 48 + (nbr % 10);
		nbr = nbr / 10;
	}
	return (str);
}