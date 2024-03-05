/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_table.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 15:27:24 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/03/05 12:17:38 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static	void	announce_exit(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_of_philosophers)
	{
		pthread_mutex_lock(&table->philos[i].l_exit);
		table->philos[i].exit = 1;
		pthread_mutex_unlock(&table->philos[i].l_exit);
		i++;
	}
}

static int	is_straved(t_philo *philo, t_table *table)
{
	int	straved;

	pthread_mutex_lock(&philo->l_meal);
	straved = ms_since_start(table->starttime_msec) - philo->last_eat
		>= table->time_to_die;
	pthread_mutex_unlock(&philo->l_meal);
	if (!straved)
		return (0);
	announce_exit(table);
	printf("%6d %d died\n", ms_since_start(philo->starttime), philo->id);
	return (1);
}

static int	check_eaten(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_of_philosophers)
	{
		pthread_mutex_lock(&table->philos[i].l_meal);
		if (table->philos[i].meals_eaten < table->required_meals)
		{
			pthread_mutex_unlock(&table->philos[i].l_meal);
			return (0);
		}
		pthread_mutex_unlock(&table->philos[i].l_meal);
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
	i = -1;
	pthread_mutex_unlock(&table->start);
	pthread_mutex_lock(&table->start);
	pthread_mutex_unlock(&table->start);
	while (1)
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
		return (single_philo(table));
	pthread_mutex_lock(&table->start);
	i = -1;
	while (++i < table->num_of_philosophers)
	{
		if (pthread_create(&table->philos[i].tid, NULL,
				philo_start, (void *) &table->philos[i]) != 0)
		{
			i = -1;
			announce_exit(table);
			pthread_mutex_unlock(&table->start);
			printf("Error: Couldn't create pthread\n");
			break ;
		}
	}
	if (i >= 1)
		table_start(table);
	i = -1;
	while (++i < table->num_of_philosophers)
		pthread_join(table->philos[i].tid, NULL);
}
