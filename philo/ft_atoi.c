/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 18:31:25 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/10 18:57:16 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isonlydigits(char *str)
{
	while (*str)
	{
		if (!(*str >= '0' && *str <= '9'))
			return (0);
		str++;
	}
	return (1);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

static int	ft_isspace(char c)
{
	if (c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r' || c == ' ')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	long	i;
	int		sign;

	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '-')
		sign *= -1;
	if (*str == '+' || *str == '-')
		str++;
	i = 0;
	while (ft_isdigit(*str))
	{
		i = i * 10 + *str - '0';
		if (i < 0)
		{
			if (sign == 1)
				return (-1);
			return (0);
		}
		str++;
	}
	return ((int) i * sign);
}
