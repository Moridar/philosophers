/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_table.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 15:27:24 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/29 11:09:18 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	is_straved(t_philo *philo, t_table *table)
{
	if (ms_since_start(philo) - philo->last_eat < philo->table->time_to_die)
		return (0);
	table->exit = 1;
	usleep(100);
	printf("%6d %d died\n", ms_since_start(philo), philo->id);
	return (1);
}

static void	table_start(t_table *table)
{
	int		i;

	table->starttime_msec = now_msec();
	table->exit = 0;
	while (table->exit == 0)
	{
		i = -1;
		while (++i < table->num_of_philosophers)
			if (is_straved(&table->philos[i], table))
				return ;
		i = 0;
		while (table->required_meals >= 0 && i < table->num_of_philosophers)
		{
			if (table->philos[i].meals_eaten < table->required_meals)
				break ;
			if (++i == table->num_of_philosophers)
				table->exit = 1;
		}
		usleep(500);
	}
}

void	event_start(t_table *table)
{
	int				i;

	if (table->num_of_philosophers == 1)
	{
		pthread_create(&table->philos[0].tid, NULL,
			single_philo_start, &table->philos[0]);
		pthread_join(table->philos[0].tid, NULL);
		return ;
	}
	i = 0;
	while (i < table->num_of_philosophers)
	{
		pthread_create(&table->philos[i].tid, NULL,
			philo_start, (void *) &table->philos[i]);
		i++;
	}
	table_start(table);
	while (--i > 0)
		pthread_join(table->philos[i].tid, NULL);
}
