/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 15:47:15 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/10 19:33:26 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	strave(struct timeval last_eat, int time_to_die)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	if (current_time.tv_sec > last_eat.tv_sec)
		return (1);
	if (current_time.tv_usec > last_eat.tv_usec)
		return (1);
	return (0);
}

void	philo_start(int id, t_table *table)
{
	t_philo	philo;
	struct timeval	current_time;

	philo.id = id + 1;
	philo.is_alive = 1;
	philo.left_fork = &table->forks[id];
	philo.right_fork = &table->forks[(id + 1) % table->num_of_philosophers];
	gettimeofday(&philo.last_eat, NULL);
	while (philo.is_alive)
	{
		while (philo.is_alive)
		{
			if (philo.left_fork && philo.right_fork)
				break ;
			if (starve(philo.last_eat, table->time_to_die))
				philo.is_alive = 0;
			philo_eat(&philo);
			usleep(table->time_to_eat);
		}
	}
}

static int	initialise(int argc, char **argv, t_table *table)
{
	int		i;

	i = 0;
	while (++i < argc)
	{
		if (!ft_isonlydigits(argv[i]))
			return (0);
	}
	table->num_of_philosophers = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->forks = malloc(sizeof(int) * table->num_of_philosophers);
	table->philos = malloc(sizeof(t_philo) * table->num_of_philosophers);
	if (argc == 6)
		table->num_of_times_each_philo_must_eat = ft_atoi(argv[5]);
	else
		table->num_of_times_each_philo_must_eat = -1;
	return (1);
}

int	main(int argc, char **argv)
{
	struct timeval	current_time;
	t_table			table;

	if (argc == 5 || argc == 6)
		if (!initialise(argc, argv, &table))
			return (printf("Error: one more more wrong argument\n"));
	else
		return (printf("Error: wrong number of arguments\n"));
	printf("time: %d\n", gettimeofday(&current_time, NULL));
	printf("time: %ld, msec: %ld\n", current_time.tv_sec, current_time.tv_usec);
	gettimeofday(&current_time, NULL);
	free(table.forks);
	free(table.philos);
	return (0);
}
