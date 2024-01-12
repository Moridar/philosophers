/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 15:48:00 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/12 16:06:05 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_philo
{
	int				id;
	int				is_alive;
	int				*left_fork;
	int				*right_fork;
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

void	philo_eat(t_philo *philo, t_table *table);
int		is_straved(t_philo *philo, int time_to_die);
int		get_ms_since_start(void);
int		ft_isonlydigits(char *str);
int		ft_atoi(const char *str);
