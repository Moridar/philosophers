/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 15:48:00 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/03/04 16:59:16 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct s_table	t_table;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	int				last_eat;
	int				exit;
	pthread_t		tid;
	pthread_mutex_t	data;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
	int				time_to_eat;
	int				time_to_sleep;
	long			starttime;
	t_table			*table;
}	t_philo;

typedef struct s_table
{
	int				num_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				required_meals;
	int				exit;
	long			starttime_msec;
	pthread_mutex_t	start;
	pthread_mutex_t	*forks;
	t_philo			*philos;
}	t_table;

void	*single_philo_start(void *arg);
void	*philo_start(void *arg);
void	event_start(t_table *table);
long	now_msec(void);
int		ms_since_start(long starttime);
long	set_alarm(int sleeptime);
int		ft_isonlydigits(char *str);
int		ft_atoi(const char *str);
