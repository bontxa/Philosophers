/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboncine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 08:59:58 by aboncine          #+#    #+#             */
/*   Updated: 2022/12/19 17:41:24 by aboncine         ###   ########.fr       */
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
pthread_t	thr_arr;
t_struct	*box;
}	t_philo;

typedef struct s_struct {
int	philo;
int	die;
int	eat;
int	sleep;
int	times;
int	i;
pthread_mutex_t	*mut_arr;
t_philo	**philos;
}	t_struct;

#endif
