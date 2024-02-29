/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 15:48:00 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/29 10:16:30 by bsyvasal         ###   ########.fr       */
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
	pthread_t		tid;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
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
	pthread_mutex_t	*forks;
	pthread_mutex_t	start;
	pthread_t		tid;
	t_philo			*philos;
}	t_table;

long	set_alarm(int sleeptime);
void	*philo_start(void *arg);
void	*single_philo_start(void *arg);
void	*table_start(void *arg);
long	now_msec(void);
int		ms_since_start(t_philo *philo);
int		ft_isonlydigits(char *str);
int		ft_atoi(const char *str);
