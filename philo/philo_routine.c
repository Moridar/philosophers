/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 12:35:51 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/22 16:04:25 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	philo_sleep(int duration, t_philo *philo)
{
	int	wakeup;

	wakeup = (since_start(philo) + duration);
	wakeup -= 1;
	while (since_start(philo) <= wakeup)
	{
		if (philo->table->exit)
			return (0);
		usleep(500);
	}
	return (1);
}

static int	philo_eat(t_philo *philo)
{
	int				time_since_start;

	if (pthread_mutex_lock(philo->left_fork) != 0)
		return (0);
	if (pthread_mutex_lock(philo->right_fork) != 0)
	{
		pthread_mutex_unlock(philo->left_fork);
		return (0);
	}
	philo->meals_eaten++;
	if (philo->table->exit)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (0);
	}
	time_since_start = since_start(philo);
	philo->last_eat = time_since_start;
	printf("%6d %d has taken a fork\n", time_since_start, philo->id);
	printf("%6d %d has taken a fork\n", time_since_start, philo->id);
	printf("%6d %d is eating\n", time_since_start, philo->id);
	philo_sleep(philo->table->time_to_eat, philo);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (1);
}

void	*philo_start(void *arg)
{
	t_philo			*philo;

	philo = arg;
	philo->last_eat = since_start(philo);
	while (philo->table->exit == 0)
	{
		while (philo->table->exit == 0 && philo_eat(philo) == 0)
			;
		if (philo->table->exit)
			return (NULL);
		printf("%6d %d is sleeping\n", since_start(philo), philo->id);
		philo_sleep(philo->table->time_to_sleep, philo);
		if (philo->table->exit)
			return (NULL);
		printf("%6d %d is thinking\n", since_start(philo), philo->id);
	}
	return (NULL);
}
