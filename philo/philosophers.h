/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 15:48:00 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/28 14:13:29 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct s_table	t_table;

typedef struct s_fork
{
	int				free;
	pthread_mutex_t	lock;
}	t_fork;

typedef struct s_philo
{
	int			id;
	int			meals_eaten;
	int			last_eat;
	pthread_t	tid;
	t_fork		*right_fork;
	t_fork		*left_fork;
	t_table		*table;
}	t_philo;

typedef struct s_table
{
	int			num_of_philosophers;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			required_meals;
	int			exit;
	long		starttime_usec;
	t_fork		*forks;
	pthread_t	tid;
	t_philo		*philos;
}	t_table;

void	*philo_start(void *arg);
void	*single_philo_start(void *arg);
void	*table_start(void *arg);
long	now_usec(void);
int		ms_since_start(t_philo *philo);
int		ft_isonlydigits(char *str);
int		ft_atoi(const char *str);
