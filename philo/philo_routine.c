/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 12:35:51 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/28 17:44:01 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	philo_sleep(long wakeup, t_philo *philo)
{
	while (wakeup > now_usec() + 500)
	{
		if (philo->table->exit)
			return (0);
		usleep(500);
	}
	while (wakeup > now_usec() + 100)
	{
		if (philo->table->exit)
			return (0);
		usleep(100);
	}
	return (1);
}

static int	get_fork(t_philo *philo)
{
	if (pthread_mutex_lock(philo->left_fork) != 0)
		return (0);
	if (philo->table->exit)
		return (1);
	printf("%6d %d has taken a fork\n", ms_since_start(philo), philo->id);
	if (pthread_mutex_lock(philo->right_fork) != 0)
		return (0);
	if (philo->table->exit)
		return (1);
	printf("%6d %d has taken a fork\n", ms_since_start(philo), philo->id);
	return (1);
}

static void	philo_eat(t_philo *philo)
{
	long	alarm;

	if (get_fork(philo) == 0)
	{
		philo->table->exit = 2;
		printf("Error: lock\n");
	}
	alarm = set_alarm(philo->table->time_to_eat);
	philo->last_eat = ms_since_start(philo);
	philo->meals_eaten++;
	if (philo->table->exit)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return ;
	}
	printf("%6d %d is eating\n", ms_since_start(philo), philo->id);
	philo_sleep(alarm, philo);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	*philo_start(void *arg)
{
	t_philo			*philo;
	long			alarm;

	philo = arg;
	pthread_mutex_lock(&philo->table->start);
	pthread_mutex_unlock(&philo->table->start);
	if (philo->id % 2 == 0)
		usleep(150);
	while (philo->table->exit == 0)
	{
		philo_eat(philo);
		if (philo->table->exit)
			return (NULL);
		alarm = set_alarm(philo->table->time_to_sleep);
		printf("%6d %d is sleeping\n", ms_since_start(philo), philo->id);
		philo_sleep(alarm, philo);
		if (philo->table->exit)
			return (NULL);
		printf("%6d %d is thinking\n", ms_since_start(philo), philo->id);
	}
	return (NULL);
}

void	*single_philo_start(void *arg)
{
	t_philo			*philo;

	philo = arg;
	printf("%6d 1 has taken a fork\n", 0);
	usleep(philo->table->time_to_die * 1000);
	printf("%6d 1 has died\n", philo->table->time_to_die);
	return (NULL);
}
