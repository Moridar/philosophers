/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 12:35:51 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/28 15:22:05 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	philo_sleep(long wakeup, t_philo *philo)
{
	long	timeleft;

	while (1)
	{
		timeleft = wakeup - now_usec();
		if (philo->table->exit)
			return (0);
		if (timeleft < 1500)
			break ;
		if (timeleft > 0)
			usleep(1000);
	}
	timeleft = wakeup - now_usec();
	if (timeleft > 0)
		usleep(timeleft);
	return (1);
}

static int	get_fork(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->left_fork->lock) != 0)
		return (0);
	if (!philo->left_fork->free)
	{
		pthread_mutex_unlock(&philo->left_fork->lock);
		return (0);
	}
	philo->left_fork->free = 0;
	pthread_mutex_unlock(&philo->left_fork->lock);
	if (pthread_mutex_lock(&philo->right_fork->lock) != 0)
		return (0);
	if (!philo->right_fork->free)
	{
		philo->left_fork->free = 1;
		pthread_mutex_unlock(&philo->right_fork->lock);
		return (0);
	}
	philo->right_fork->free = 0;
	pthread_mutex_unlock(&philo->right_fork->lock);
	return (1);
}

static int	philo_eat(t_philo *philo)
{
	long	alarm;

	usleep(100);
	while (philo->table->exit == 0 && get_fork(philo) == 0)
		usleep(5);
	alarm = set_alarm(philo->table->time_to_eat);
	philo->last_eat = ms_since_start(philo);
	philo->meals_eaten++;
	if (philo->table->exit)
		return (0);
	printf("%6d %d has taken a fork\n", ms_since_start(philo), philo->id);
	printf("%6d %d has taken a fork\n", ms_since_start(philo), philo->id);
	printf("%6d %d is eating\n", ms_since_start(philo), philo->id);
	philo_sleep(alarm, philo);
	philo->left_fork->free = 1;
	philo->right_fork->free = 1;
	return (1);
}

void	*philo_start(void *arg)
{
	t_philo			*philo;
	long			alarm;

	philo = arg;
	while (philo->table->exit == -1)
		usleep(10);
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
