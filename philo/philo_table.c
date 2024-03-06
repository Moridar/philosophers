/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_table.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 15:27:24 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/03/06 12:14:58 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	announce_exit(t_table *table, int deadphilo)
{
	int	i;

	i = 0;
	while (i < table->num_of_philosophers)
		if (pthread_mutex_lock(&table->philos[i++].l_exit) != 0)
			return (0);
	i = 0;
	if (deadphilo)
		printf("%6d %d died\n",
			ms_since_start(table->starttime_msec), deadphilo);
	while (i < table->num_of_philosophers)
	{
		table->philos[i].exit = 1;
		if (pthread_mutex_unlock(&table->philos[i].l_exit) != 0)
			return (0);
		i++;
	}
	return (1);
}

static int	is_straved(t_table *table)
{
	int	straved;
	int	last_eat;
	int	i;

	i = 0;
	while (i < table->num_of_philosophers)
	{
		if (pthread_mutex_lock(&table->philos[i].l_meal) != 0)
			errexit(0, "Error: lock", table);
		last_eat = table->philos[i].last_eat;
		if (pthread_mutex_unlock(&table->philos[i].l_meal) != 0)
			errexit(0, "Error: unlock", table);
		straved = ms_since_start(table->starttime_msec) - last_eat
			>= table->time_to_die;
		i++;
		if (straved)
		{
			announce_exit(table, i);
			return (1);
		}
	}
	return (0);
}

static int	check_eaten(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_of_philosophers)
	{
		if (pthread_mutex_lock(&table->philos[i].l_meal) != 0)
			return (errexit(0, "Error: lock", table));
		if (table->philos[i].meals_eaten < table->required_meals)
		{
			if (pthread_mutex_unlock(&table->philos[i].l_meal) != 0)
				errexit(0, "Error: unlock", table);
			return (0);
		}
		if (pthread_mutex_unlock(&table->philos[i].l_meal) != 0)
			return (errexit(0, "Error: unlock", table));
		i++;
	}
	announce_exit(table, 0);
	return (1);
}

static int	table_start(t_table *table)
{
	table->starttime_msec = now_msec();
	if (pthread_mutex_unlock(&table->lock) != 0)
		return (0);
	while (1)
	{
		if (is_straved(table))
			return (1);
		if (table->required_meals >= 0 && check_eaten(table))
			return (1);
		if (check_exit(table))
			return (announce_exit(table, 0));
		usleep(500);
	}
	return (0);
}

void	event_start(t_table *table)
{
	int				i;

	if (table->num_of_philosophers == 1)
		return (single_philo(table));
	if (pthread_mutex_lock(&table->lock) != 0 && errmsg(1, "Error: lock"))
		return ;
	i = -1;
	while (++i < table->num_of_philosophers)
	{
		if (pthread_create(&table->philos[i].tid, NULL,
				philo_start, (void *) &table->philos[i]) != 0)
		{
			announce_exit(table, 0);
			pthread_mutex_unlock(&table->lock);
			printf("Error: Couldn't create pthread\n");
			break ;
		}
	}
	if (i == table->num_of_philosophers && table_start(table) == 0)
		return ;
	i = -1;
	while (++i < table->num_of_philosophers)
		pthread_join(table->philos[i].tid, NULL);
}
