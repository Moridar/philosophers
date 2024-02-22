/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 15:47:15 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/22 16:49:37 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	event_start(t_table *table)
{
	int				i;
	struct timeval	curr;

	i = 0;
	gettimeofday(&curr, NULL);
	table->starttime = (curr.tv_sec * 1000) + (curr.tv_usec / 1000);
	while (i < table->num_of_philosophers)
	{
		pthread_create(&table->philos[i].tid, NULL,
			philo_start, (void *) &table->philos[i]);
		i++;
	}
	pthread_create(&table->tid, NULL, table_start, (void *) table);
	pthread_join(table->tid, NULL);
	table->exit = 1;
	while (--i > 0)
		pthread_detach(table->philos[i].tid);
}

static int	philo_initialise(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->num_of_philosophers)
	{
		table->philos[i].id = i + 1;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork
			= &table->forks[(i + 1) % table->num_of_philosophers];
		table->philos[i].table = table;
		table->philos[i].meals_eaten = 0;
	}
	return (1);
}

static int	forks_initialise(t_table *table)
{
	int	i;

	table->forks = malloc(sizeof(pthread_mutex_t) * table->num_of_philosophers);
	if (!table->forks)
		return (0);
	i = 0;
	while (i < table->num_of_philosophers)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&table->forks[i]);
			printf("Error: Mutex\n");
			return (0);
		}
		i++;
	}
	return (1);
}

static int	table_initialise(int argc, char **argv, t_table *table)
{
	table->exit = 0;
	table->num_of_philosophers = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->required_meals = -1;
	if (argc == 6)
		table->required_meals = ft_atoi(argv[5]);
	table->philos = malloc(sizeof(t_philo) * table->num_of_philosophers);
	if (table->philos == NULL)
		return (0);
	if (forks_initialise(table) == 0)
	{
		free(table->forks);
		free(table->philos);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_table			table;
	int				i;

	if (argc < 5 || argc > 6)
		return (printf("Error: Wrong number of arguments\n"));
	i = 0;
	while (++i < argc)
		if (!ft_isonlydigits(argv[i]))
			return (printf("Error: Non-numerical argument\n"));
	if (!table_initialise(argc, argv, &table))
		return (1);
	philo_initialise(&table);
	event_start(&table);
	i = 0;
	while (i < table.num_of_philosophers)
		pthread_mutex_destroy(&table.forks[i++]);
	free(table.forks);
	free(table.philos);
	return (0);
}
