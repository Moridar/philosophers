/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 12:35:51 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/03/10 00:38:06 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	print_checkexit(t_philo *philo, char *msg)
{
	int	exit;

	if (pthread_mutex_lock(&philo->l_exit) != 0)
		return (errexit(1, "Error: lock", philo->table));
	exit = philo->exit;
	if (pthread_mutex_unlock(&philo->l_exit) != 0)
		return (errexit(1, "Error: unlock", philo->table));
	if (!exit && msg)
		printf("%6d %d %s\n", ms_since_start(philo->starttime), philo->id, msg);
	return (exit);
}

static int	philo_sleep(long wakeup, t_philo *philo)
{
	while (wakeup > now_msec() + 1)
	{
		if (print_checkexit(philo, NULL))
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
		return (errexit(0, "Error: lock", philo->table));
	print_checkexit(philo, "has taken a fork");
	if (pthread_mutex_lock(philo->right_fork) != 0)
		return (errexit(0, "Error: lock", philo->table));
	*alarm = set_alarm(philo->time_to_eat);
	print_checkexit(philo, "has taken a fork");
	return (1);
}

static void	philo_eat(t_philo *philo)
{
	long	alarm;

	if (get_fork(philo, &alarm) == 0)
		return ;
	if (pthread_mutex_lock(&philo->l_meal) != 0)
		errexit(0, "Error: lock", philo->table);
	print_checkexit(philo, "is eating");
	philo->last_eat = ms_since_start(philo->starttime);
	philo->meals_eaten++;
	if (pthread_mutex_unlock(&philo->l_meal) != 0)
		errexit(0, "Error: unlock", philo->table);
	philo_sleep(alarm, philo);
	if (pthread_mutex_unlock(philo->left_fork) != 0)
		errexit(0, "Error: unlock", philo->table);
	if (pthread_mutex_unlock(philo->right_fork) != 0)
		errexit(0, "Error: unlock", philo->table);
}

void	*philo_start(void *arg)
{
	t_philo			*philo;
	long			alarm;

	philo = arg;
	if (pthread_mutex_lock(&philo->table->lock) != 0)
		errexit(0, "Error: lock", philo->table);
	philo->starttime = philo->table->starttime_msec;
	if (pthread_mutex_unlock(&philo->table->lock) != 0)
		errexit(0, "Error: unlock", philo->table);
	if (philo->id % 2 == 0)
		usleep(10000);
	while (1)
	{
		philo_eat(philo);
		alarm = set_alarm(philo->time_to_sleep);
		if (print_checkexit(philo, "is sleeping"))
			return (NULL);
		philo_sleep(alarm, philo);
		if (print_checkexit(philo, "is thinking"))
			return (NULL);
	}
	return (NULL);
}
