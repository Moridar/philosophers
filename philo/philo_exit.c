/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 10:10:51 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/03/06 11:46:37 by bsyvasal         ###   ########.fr       */
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
	i = 0;
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
		pthread_mutex_destroy(&table->lock);
	free(table->philos);
}

int	errmsg(int value, char *msg)
{
	printf("%s\n", msg);
	return (value);
}

void	thread_exit(t_table *table)
{
	pthread_mutex_lock(&table->lock);
	table->exit = 1;
	pthread_mutex_unlock(&table->lock);
}

int	check_exit(t_table *table)
{
	int	exit;

	if (pthread_mutex_lock(&table->lock) != 0)
		return (errmsg(1, "Error: lock"));
	exit = table->exit;
	if (pthread_mutex_unlock(&table->lock) != 0)
		return (errmsg(1, "Error unlock"));
	return (exit);
}

int	errexit(int value, char *msg, t_table *table)
{
	printf("%s\n", msg);
	thread_exit(table);
	return (value);
}
