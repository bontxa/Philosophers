/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboncine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 08:59:58 by aboncine          #+#    #+#             */
/*   Updated: 2022/12/27 17:02:14 by aboncine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# define INIT_TIME philo->box->init_time
# define ID philo->phi_id
# define MUTEX &philo->box->mut_arr
# define DIED get_time() - box->init_time, box->philos[i]->phi_id + 1

typedef struct s_struct	t_struct;

typedef struct s_philo {
int	phi_id;
int	eat_count;
time_t	start_eat;
pthread_t	thr_arr;
t_struct	*box;
}	t_philo;

typedef struct s_struct {
int	philo;
time_t	init_time;
time_t	die;
time_t	eat;
time_t	sleep;
int	times;
int	argc;
pthread_mutex_t	*mut_arr;
t_philo	**philos;
}	t_struct;

void	free_for_all(t_struct *box);
void	*my_malloc(int size);
time_t	get_time(void);
void	append_time(time_t time);
int		ft_atoi(const char *nptr);
void	*is_eating(void *arg);

#endif
