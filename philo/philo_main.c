/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 15:47:15 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/03/06 10:21:20 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	initalise(int argc, char **argv, t_table *table)
{
	if (!table_initialise(argc, argv, table))
		return (0);
	if (locks_initialise(table) == 0)
		return (0);
	if (philo_initialise(table) == 0)
		return (0);
	return (1);
}

static int	validate_input(int argc, char **argv)
{
	int	i;

	if (argc <= 1)
		printf("Usage: ./philo num_of_philo time_to_die time_to_eat "
			"time_to_sleep [num_of_meals]\n");
	if (argc < 5 || argc > 6)
		return (errmsg(0, "Error: Number of arguments\n"));
	i = 0;
	while (++i < argc)
		if (!ft_isonlydigits(argv[i]))
			return (errmsg(0, "Error: Illegal arguments\n"));
	return (1);
}

int	main(int argc, char **argv)
{
	t_table			table;

	if (validate_input(argc, argv) == 0)
		return (1);
	if (initalise(argc, argv, &table) == 0)
		return (1);
	event_start(&table);
	clean_memory(5, &table);
	return (0);
}
