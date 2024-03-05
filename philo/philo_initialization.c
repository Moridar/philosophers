/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 10:43:14 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/03/05 11:09:30 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	philo_initialise(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->num_of_philosophers)
	{
		table->philos[i].id = i + 1;
		table->philos[i].right_fork = &table->forks[i];
		table->philos[i].left_fork
			= &table->forks[(i + 1) % table->num_of_philosophers];
		table->philos[i].table = table;
		table->philos[i].meals_eaten = 0;
		table->philos[i].last_eat = 0;
		table->philos[i].time_to_eat = table->time_to_eat;
		table->philos[i].time_to_sleep = table->time_to_sleep;
		table->philos[i].exit = 0;
	}
	return (1);
}

static int	forks_initialise(t_table *table)
{
	int	i;

	table->forks = malloc(sizeof(pthread_mutex_t) * table->num_of_philosophers);
	if (!table->forks)
	{
		clean_memory(1, table);
		return (errmsg(0, "Error: Malloc\n"));
	}
	i = -1;
	while (++i < table->num_of_philosophers)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&table->forks[i]);
			clean_memory(2, table);
			return (errmsg(0, "Error: Mutex\n"));
		}
	}
	return (1);
}

static int	philo_locks_initialise(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->num_of_philosophers)
	{
		if (pthread_mutex_init(&(table->philos[i].l_exit), NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&table->philos[i].l_exit);
			clean_memory(3, table);
			return (errmsg(0, "Error: Mutex\n"));
		}
	}
	i = -1;
	while (++i < table->num_of_philosophers)
	{
		if (pthread_mutex_init(&(table->philos[i].l_meal), NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&table->philos[i].l_meal);
			clean_memory(4, table);
			return (errmsg(0, "Error: Mutex\n"));
		}
	}
	return (1);
}

int	locks_initialise(t_table *table)
{
	if (pthread_mutex_init(&table->start, NULL) != 0)
	{
		free(table->philos);
		return (errmsg(0, "Error: Mutex\n"));
	}
	if (forks_initialise(table) == 0)
		return (0);
	if (philo_locks_initialise(table) == 0)
		return (0);
	return (1);
}

int	table_initialise(int argc, char **argv, t_table *table)
{
	table->num_of_philosophers = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->required_meals = -1;
	if (argc == 6)
		table->required_meals = ft_atoi(argv[5]);
	if (table->num_of_philosophers <= 0 || table->time_to_die <= 0
		|| table->time_to_eat <= 0 || table->time_to_sleep <= 0
		|| (argc == 6 && table->required_meals < 0))
		return (errmsg(0,
				"Error: Arguments must be non-zero positive number\n"));
	table->philos = malloc(sizeof(t_philo) * table->num_of_philosophers);
	if (table->philos == NULL)
		return (errmsg(0, "Error: Malloc\n"));
	return (1);
}
