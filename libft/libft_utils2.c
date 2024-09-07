/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <elel-bah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:57:10 by elel-bah          #+#    #+#             */
/*   Updated: 2024/08/29 16:57:29 by elel-bah         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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