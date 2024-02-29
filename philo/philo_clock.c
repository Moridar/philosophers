/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_clock.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 11:01:09 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/29 11:01:28 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	now_msec(void)
{
	struct timeval	curr;

	gettimeofday(&curr, NULL);
	return (curr.tv_sec * 1000 + curr.tv_usec / 1000);
}

int	ms_since_start(t_philo *philo)
{
	return ((now_msec() - philo->table->starttime_msec));
}

long	set_alarm(int sleeptime)
{
	return (now_msec() + sleeptime);
}
