/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 10:10:51 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/03/05 11:06:47 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	clean_memory(int num, t_table *table)
{
	int	i;

	i = 0;
	if (num >= 5)
		while (i < table->num_of_philosophers)
			pthread_mutex_destroy(&table->philos[i++].l_meal);
	if (num >= 4)
		while (i < table->num_of_philosophers)
			pthread_mutex_destroy(&table->philos[i++].l_exit);
	i = 0;
	if (num >= 3)
		while (i < table->num_of_philosophers)
			pthread_mutex_destroy(&table->forks[i++]);
	if (num >= 2)
		free(table->forks);
	if (num >= 1)
		pthread_mutex_destroy(&table->start);
	free(table->philos);
}

int	errmsg(int value, char *msg)
{
	printf("%s\n", msg);
	return (value);
}
