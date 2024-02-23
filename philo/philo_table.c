/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_table.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 15:27:24 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/23 11:12:43 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

double	now_usec(void)
{
	double			now_usec;
	struct timeval	curr;

	gettimeofday(&curr, NULL);
	now_usec = (curr.tv_sec % 1000000 * 1000000) + (curr.tv_usec);
	return (now_usec);
}

int	ms_since_start(t_philo *philo)
{
	return ((now_usec() - philo->table->starttime_usec) / 1000);
}

static int	is_straved(t_philo *philo)
{
	if (ms_since_start(philo) - philo->last_eat < philo->table->time_to_die)
		return (0);
	printf("%6d philo %d died\n",ms_since_start(philo), philo->id);
	return (1);
}

void	*table_start(void *arg)
{
	t_table	*table;
	int		i;

	table = arg;
	while (table->exit == 0)
	{
		i = 0;
		while (i < table->num_of_philosophers)
		{
			if (is_straved(&table->philos[i]))
				return (NULL);
			i++;
		}
		i = 0;
		while (table->required_meals >= 0 && i < table->num_of_philosophers)
		{
			if (table->philos[i].meals_eaten < table->required_meals)
				break ;
			if (++i == table->num_of_philosophers)
			{
				printf("everyone had %d meals\n", table->required_meals);
				return (NULL);
			}
		}
		usleep(1000);
	}
	return (NULL);
}
