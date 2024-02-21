/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 15:48:00 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/21 15:18:03 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

struct	s_table;

typedef struct s_philo
{
	int				id;
	int				is_alive;
	int				*left_fork;
	int				*right_fork;
	int				number_of_times_to_eat;
	struct timeval	last_eat;
	pthread_t		tid;
	pthread_mutex_t	*lock;
	struct s_table	*table;
}	t_philo;

typedef struct s_table
{
	int		num_of_philosophers;
	int		*forks;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		num_of_times_each_philo_must_eat;
	int		dead_philo;
	t_philo	*philos;
}	t_table;

int		philo_eat(t_philo *philo);
int		is_straved(t_philo *philo);
int		get_ms_since_start(void);
int		ft_isonlydigits(char *str);
int		ft_atoi(const char *str);
