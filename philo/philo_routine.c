/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 12:35:51 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/28 14:33:42 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	philo_sleep(int state, int duration, t_philo *philo)
{
	long	wakeup;
	long	timeleft;

	wakeup = now_usec() + duration * 1000;
	if (state == 0)
		printf("%6d %d is eating\n", ms_since_start(philo), philo->id);
	if (state == 1)
		printf("%6d %d is sleeping\n", ms_since_start(philo), philo->id);
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
	if (timeleft > 100)
		usleep(timeleft - 100);
	return (1);
}

static int	get_fork(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->left_fork->lock) != 0)
		return (0);
	if (!philo->left_fork->free || !philo->right_fork->free)
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
	if (philo->table->exit)
		return (1);
	printf("%6d %d has taken a fork\n", ms_since_start(philo), philo->id);
	printf("%6d %d has taken a fork\n", ms_since_start(philo), philo->id);
	return (1);
}

static int	philo_eat(t_philo *philo)
{
	while (philo->table->exit == 0 && get_fork(philo) == 0)
		usleep(10);
	philo->meals_eaten++;
	if (philo->table->exit)
		return (0);
	philo->last_eat = ms_since_start(philo);
	philo_sleep(0, philo->table->time_to_eat, philo);
	philo->left_fork->free = 1;
	philo->right_fork->free = 1;
	return (1);
}

void	*philo_start(void *arg)
{
	t_philo			*philo;

	philo = arg;
	while (philo->table->exit == -1)
		usleep(10);
	while (philo->table->exit == 0)
	{
		philo_eat(philo);
		if (philo->table->exit)
			return (NULL);
		philo_sleep(1, philo->table->time_to_sleep, philo);
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
