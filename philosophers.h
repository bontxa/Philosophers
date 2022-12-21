/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboncine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 08:59:58 by aboncine          #+#    #+#             */
/*   Updated: 2022/12/21 18:20:08 by aboncine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_struct	t_struct;

typedef struct s_philo {
int	phi_id;
time_t	start_eat;
//time_t	philo_die;
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
int	i;
pthread_mutex_t	*mut_arr;
t_philo	**philos;
}	t_struct;

#endif
