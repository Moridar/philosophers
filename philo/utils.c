/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 15:27:24 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/12 16:47:09 by bsyvasal         ###   ########.fr       */
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

int	is_straved(t_philo *philo, int time_to_die)
{
	struct timeval	curr_time;

	gettimeofday(&curr_time, NULL);
	curr_time.tv_usec += (curr_time.tv_sec - philo->last_eat.tv_sec) * 1000000;
	if (curr_time.tv_usec <= philo->last_eat.tv_usec + time_to_die)
		return (0);
	philo->is_alive = 0;
	printf("%d ms: philo %d died\n", get_ms_since_start(), philo->id);
	return (1);
}

void	philo_eat(t_philo *philo, t_table *table)
{
	struct timeval	current_time;

	*philo->left_fork = 0;
	*philo->right_fork = 0;
	gettimeofday(&current_time, NULL);
	philo->last_eat = current_time;
	printf("%ld: philo %d is eating\n", current_time.tv_sec, philo->id);
	usleep(table->time_to_eat);
	*philo->left_fork = 1;
	*philo->right_fork = 1;
}
