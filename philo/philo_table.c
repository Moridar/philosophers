/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_table.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 15:27:24 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/29 10:16:43 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	now_msec(void)
{
	struct timeval	curr;

	gettimeofday(&curr, NULL);
	return (curr.tv_sec * 1000 + curr.tv_usec / 1000);
}

int	ms_since_start(t_philo *philo)
{
	return ((now_msec() - philo->table->starttime_msec));
}

long	set_alarm(int sleeptime)
{
	return (now_msec() + sleeptime);
}

static int	is_straved(t_philo *philo, t_table *table)
{
	if (ms_since_start(philo) - philo->last_eat < philo->table->time_to_die)
		return (0);
	table->exit = 1;
	printf("%6d %d died\n", ms_since_start(philo), philo->id);
	return (1);
}

void	*table_start(void *arg)
{
	int		i;
	t_table	*table;

	table = arg;
	table->starttime_msec = now_msec();
	pthread_mutex_unlock(&table->start);
	while (table->exit == 0)
	{
		i = -1;
		while (++i < table->num_of_philosophers)
			if (is_straved(&table->philos[i], table))
				return (NULL);
		i = 0;
		while (table->required_meals >= 0 && i < table->num_of_philosophers)
		{
			if (table->philos[i].meals_eaten < table->required_meals)
				break ;
			if (++i == table->num_of_philosophers)
				table->exit = 1;
		}
		usleep(1000);
	}
	return (NULL);
}
