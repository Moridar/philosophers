/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 12:35:51 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/03/05 13:16:53 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	print_checkdeath(t_philo *philo, char *msg)
{
	int	exit;

	pthread_mutex_lock(&philo->l_exit);
	exit = philo->exit;
	pthread_mutex_unlock(&philo->l_exit);
	if (!exit && msg)
		printf("%6d %d %s\n", ms_since_start(philo->starttime), philo->id, msg);
	return (exit);
}

static int	philo_sleep(long wakeup, t_philo *philo)
{
	while (wakeup > now_msec() + 1)
	{
		if (print_checkdeath(philo, NULL))
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
	print_checkdeath(philo, " has taken a fork");
	if (pthread_mutex_lock(philo->right_fork) != 0)
		return (errmsg(0, "Error: lock\n"));
	*alarm = set_alarm(philo->time_to_eat);
	print_checkdeath(philo, " has taken a fork");
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
	print_checkdeath(philo, "is eating");
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
	if (philo->id % 2 == 0)
		usleep(10000);
	while (1)
	{
		philo_eat(philo);
		alarm = set_alarm(philo->time_to_sleep);
		if (print_checkdeath(philo, "is sleeping"))
			return (NULL);
		philo_sleep(alarm, philo);
		if (print_checkdeath(philo, "is thinking"))
			return (NULL);
	}
	return (NULL);
}
