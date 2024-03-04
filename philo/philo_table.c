/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_table.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 15:27:24 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/03/04 17:00:55 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static	void	announce_exit(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_of_philosophers)
	{
		pthread_mutex_lock(&table->philos[i].data);
		table->philos[i].exit = 1;
		pthread_mutex_unlock(&table->philos[i].data);
		i++;
	}
}

static int	is_straved(t_philo *philo, t_table *table)
{
	int	straved;

	pthread_mutex_lock(&philo->data);
	straved = ms_since_start(table->starttime_msec) - philo->last_eat
		>= table->time_to_die;
	pthread_mutex_unlock(&philo->data);
	if (!straved)
		return (0);
	announce_exit(table);
	usleep(100);
	printf("%6d %d died\n", ms_since_start(philo->starttime), philo->id);
	return (1);
}

static int	check_eaten(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_of_philosophers)
	{
		pthread_mutex_lock(&table->philos[i].data);
		if (table->philos[i].meals_eaten < table->required_meals)
		{
			pthread_mutex_unlock(&table->philos[i].data);
			return (0);
		}
		pthread_mutex_unlock(&table->philos[i].data);
		i++;
	}
	if (i == table->num_of_philosophers)
		announce_exit(table);
	return (1);
}

static void	table_start(t_table *table)
{
	int		i;

	table->starttime_msec = now_msec();
	table->exit = 0;
	i = -1;
	pthread_mutex_unlock(&table->start);
	while (table->exit == 0)
	{
		i = -1;
		while (++i < table->num_of_philosophers)
			if (is_straved(&table->philos[i], table))
				return ;
		if (table->required_meals >= 0)
			if (check_eaten(table))
				return ;
		usleep(500);
	}
}

void	event_start(t_table *table)
{
	int				i;

	if (table->num_of_philosophers == 1)
	{
		if (pthread_create(&table->philos[0].tid, NULL,
				single_philo_start, &table->philos[0]) != 0)
			printf("Error: Couldn't create pthread\n");
		else
			pthread_join(table->philos[0].tid, NULL);
		return ;
	}
	i = -1;
	while (table->exit == -1 && ++i < table->num_of_philosophers)
	{
		if (pthread_create(&table->philos[i].tid, NULL,
				philo_start, (void *) &table->philos[i]) != 0)
		{
			table->exit = 1;
			printf("Error: Couldn't create pthread\n");
		}
	}
	if (table->exit == -1)
		table_start(table);
	while (--i >= 0)
		pthread_join(table->philos[i].tid, NULL);
}
