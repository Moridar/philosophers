/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_single.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 17:01:03 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/03/05 10:42:32 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	*single_philo_thread(void *arg)
{
	t_philo			*philo;

	philo = arg;
	printf("%6d 1 has taken a fork\n", 0);
	usleep(philo->table->time_to_die * 1000);
	printf("%6d 1 has died\n", philo->table->time_to_die);
	return (NULL);
}

void	single_philo(t_table *table)
{
	if (pthread_create(&table->philos[0].tid, NULL,
			single_philo_thread, &table->philos[0]) == 0)
		pthread_join(table->philos[0].tid, NULL);
	else
		printf("Error: Couldn't create pthread\n");
	return ;
}
