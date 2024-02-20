/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 15:27:24 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/20 15:07:46 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	get_ms_since_start(void)
{
	static struct timeval	start;
	struct timeval			current;
	int						time_difference;

	if (!start.tv_sec)
		gettimeofday(&start, NULL);
	gettimeofday(&current, NULL);
	time_difference = (current.tv_sec - start.tv_sec) * 1000;
	time_difference += (current.tv_usec - start.tv_usec) / 1000;
	return (time_difference);
}

int	is_straved(t_philo *philo)
{
	struct timeval	curr_time;
	int				time_to_die;

	time_to_die = philo->time_to_die * 1000;
	gettimeofday(&curr_time, NULL);
	curr_time.tv_usec += (curr_time.tv_sec - philo->last_eat.tv_sec) * 1000000;
	if (curr_time.tv_usec <= philo->last_eat.tv_usec + time_to_die)
		return (0);
	philo->is_alive = 0;
	printf("%d ms: philo %d died\n", get_ms_since_start(), philo->id);
	return (1);
}

int	philo_eat(t_philo *philo)
{
	struct timeval	current_time;

	if (*philo->left_fork == 0)
		//printf("phil %d. Lfork: %d, Rfork: %d\n", philo->id, *philo->left_fork, *philo->right_fork);
	pthread_mutex_lock(philo->lock);
	if (*philo->left_fork == 0 || *philo->right_fork == 0)
	{
		pthread_mutex_unlock(philo->lock);
		return (0);
	}
	*philo->left_fork = 0;
	*philo->right_fork = 0;
	pthread_mutex_unlock(philo->lock);
	gettimeofday(&current_time, NULL);
	philo->last_eat = current_time;
	printf("%d: philo %d is eating\n", get_ms_since_start(), philo->id);
	usleep(philo->time_to_eat * 1000);
	if (philo->table->dead_philo)
			return (1);
	printf("%d: philo %d finished eating\n", get_ms_since_start(), philo->id);
	*philo->left_fork = 1;
	*philo->right_fork = 1;
	return (1);
}
