/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 15:47:15 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/20 15:07:23 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	exit_detach(t_philo *philo)
{
	int	i;

	printf("exit\n");
	i = 0;
	while (i < philo->table->num_of_philosophers)
	{
		printf("detaching philo[%d]\n", i);
		if (philo->id != philo->table->philos[i].id)
			pthread_detach(philo->table->philos[i].tid);
		i++;
	}
}

void	*philo_start(void *arg)
{
	t_philo			*philo;

	philo = arg;
	gettimeofday(&philo->last_eat, NULL);
	while (philo->is_alive)
	{
		while (philo->is_alive)
		{
			if (philo->table->dead_philo)
			return (NULL);
			if (philo_eat(philo))
				break ;
			if (is_straved(philo))
			{
				philo->table->dead_philo = 1;
				return (NULL);
			}
		}
		if (philo->table->dead_philo)
			return (NULL);
		printf("%d: philo %d is sleeping\n", get_ms_since_start(), philo->id);
		usleep(philo->time_to_sleep * 1000);
		if (philo->table->dead_philo)
			return (NULL);
		printf("%d: philo %d is thinking\n", get_ms_since_start(), philo->id);
	}
	return (NULL);
}

static void	event_start(t_table *table)
{
	int		i;

	i = 0;
	while (i < table->num_of_philosophers)
	{
		printf("i: %d\n", i);
		pthread_create(&table->philos[i].tid, NULL,
			philo_start, (void *) &table->philos[i]);
		i++;
	}
	i = 0;
	while (i < table->num_of_philosophers)
	{
		pthread_join(table->philos[i].tid, NULL);
		i++;
	}
}

static int	init_philos(t_table *table, pthread_mutex_t	*lock)
{
	int	i;

	i = -1;
	while (++i < table->num_of_philosophers)
	{
		table->forks[i] = 1;
		table->philos[i].id = i + 1;
		table->philos[i].is_alive = 1;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork
			= &table->forks[i + 1 % table->num_of_philosophers];
		table->philos[i].time_to_die = table->time_to_die;
		table->philos[i].time_to_eat = table->time_to_eat;
		table->philos[i].time_to_sleep = table->time_to_sleep;
		table->philos[i].lock = lock;
		table->philos[i].table = table;
	}
	return (1);
}

static int	initialise(int argc, char **argv, t_table *table)
{
	int		i;

	i = 0;
	while (++i < argc)
		if (!ft_isonlydigits(argv[i]))
			return (0);
	table->dead_philo = 0;
	table->num_of_philosophers = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->forks = malloc(sizeof(int) * table->num_of_philosophers);
	if (!table->forks)
		return (0);
	table->philos = malloc(sizeof(t_philo) * table->num_of_philosophers);
	if (!table->philos)
	{
		free(table->forks);
		return (0);
	}
	if (argc == 6)
		table->num_of_times_each_philo_must_eat = ft_atoi(argv[5]);
	else
		table->num_of_times_each_philo_must_eat = -1;
	return (1);
}

int	main(int argc, char **argv)
{
	t_table			table;
	pthread_mutex_t	lock;

	if (pthread_mutex_init(&lock, NULL) != 0)
		return (printf("mutex init failed\n"));
	if (argc == 5 || argc == 6)
		if (!initialise(argc, argv, &table))
			return (printf("Error: one more more wrong argument\n"));
	if (argc < 5 || argc > 6)
		return (printf("Error: wrong number of arguments\n"));
	init_philos(&table, &lock);
	event_start(&table);
	free(table.forks);
	free(table.philos);
	pthread_mutex_destroy(&lock);
	return (0);
}
