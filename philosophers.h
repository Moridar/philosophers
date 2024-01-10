/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 15:48:00 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/10 19:19:32 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_philo
{
	int				id;
	int				is_alive;
	int				left_fork;
	int				right_fork;
	struct timeval	last_eat;
}	t_philo;

typedef struct s_table
{
	int		num_of_philosophers;
	int		*forks;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		num_of_times_each_philo_must_eat;
	t_philo	*philos;
}	t_table;

struct timeval