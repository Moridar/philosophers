/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 12:35:51 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/03/05 11:38:48 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	is_exit(t_philo *philo)
{
	int	exit;

	pthread_mutex_lock(&philo->l_exit);
	exit = philo->exit;
	pthread_mutex_unlock(&philo->l_exit);
	return (exit);
}

static int	philo_sleep(long wakeup, t_philo *philo)
{
	while (wakeup > now_msec() + 1)
	{
		if (is_exit(philo))
			return (0);
		usleep(500);
	}
	while (wakeup > now_msec())
		usleep(100);
	return (1);
}

static int	get_fork(t_philo *philo, long *alarm)
{
	if (pthread_mutex_lock(philo->left_fork) != 0)
		return (errmsg(0, "Error: lock\n"));
	if (is_exit(philo))
	{
		pthread_mutex_unlock(philo->left_fork);
		return (0);
	}
	printf("%6d %d has taken a fork\n",
		ms_since_start(philo->starttime), philo->id);
	if (pthread_mutex_lock(philo->right_fork) != 0)
		return (errmsg(0, "Error: lock\n"));
	*alarm = set_alarm(philo->time_to_eat);
	if (is_exit(philo))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (0);
	}
	printf("%6d %d has taken a fork\n",
		ms_since_start(philo->starttime), philo->id);
	return (1);
}

static void	philo_eat(t_philo *philo)
{
	long	alarm;

	if (get_fork(philo, &alarm) == 0)
		return ;
	pthread_mutex_lock(&philo->l_meal);
	philo->last_eat = ms_since_start(philo->starttime);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->l_meal);
	if (is_exit(philo))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return ;
	}
	printf("%6d %d is eating\n", ms_since_start(philo->starttime), philo->id);
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
	philo->starttime = philo->table->starttime_msec;
	pthread_mutex_unlock(&philo->table->start);
	if (!is_exit(philo) && philo->id % 2 == 0)
		usleep(10000);
	while (is_exit(philo) == 0)
	{
		philo_eat(philo);
		if (is_exit(philo))
			return (NULL);
		alarm = set_alarm(philo->time_to_sleep);
		printf("%6d %d is sleeping\n",
			ms_since_start(philo->starttime), philo->id);
		philo_sleep(alarm, philo);
		if (is_exit(philo))
			return (NULL);
		printf("%6d %d is thinking\n",
			ms_since_start(philo->starttime), philo->id);
	}
	return (NULL);
}
