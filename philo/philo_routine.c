/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 12:35:51 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/27 16:47:42 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	philo_sleep(int duration, t_philo *philo)
{
	double	wakeup;
	double	timeleft;

	wakeup = now_usec() + duration * 1000 - 100;
	while (1)
	{
		timeleft = wakeup - now_usec();
		if (philo->table->exit)
			return (0);
		if (timeleft < 500)
			break ;
		usleep(500);
	}
	if (timeleft > 0)
		usleep(timeleft);
	return (1);
}

static int	philo_eat(t_philo *philo)
{
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
	philo->last_eat = ms_since_start(philo);
	printf("%6d %d has taken a fork\n%6d %d has taken a fork\n"
		"%6d %d is eating\n", philo->last_eat, philo->id,
		philo->last_eat, philo->id, philo->last_eat, philo->id);
	philo_sleep(philo->table->time_to_eat, philo);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (1);
}

void	*philo_start(void *arg)
{
	t_philo			*philo;

	philo = arg;
	philo->last_eat = ms_since_start(philo);
	while (philo->table->exit == 0)
	{
		while (philo->table->exit == 0 && philo_eat(philo) == 0)
			;
		if (philo->table->exit)
			return (NULL);
		printf("%6d %d is sleeping\n", ms_since_start(philo), philo->id);
		if (philo_sleep(philo->table->time_to_sleep, philo) == 0)
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
