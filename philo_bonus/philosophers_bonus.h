/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboncine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 08:59:58 by aboncine          #+#    #+#             */
/*   Updated: 2023/01/04 17:33:18 by aboncine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <semaphore.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>

typedef struct s_struct	t_struct;

typedef struct s_philo {
	int			phi_id;
	int			eat_count;
	time_t		start_eat;
	pthread_t	check;
	pid_t		pid;
	t_struct	*box;
}	t_philo;

typedef struct s_struct {
	int		philo;
	int		exit;
	int		all_ate;
	int		times;
	int		argc;
	time_t	init_time;
	time_t	die;
	time_t	eat;
	time_t	sleep;
	sem_t	*sem;
	sem_t	*write;
	sem_t	*eating;
	sem_t	*death;
	t_philo	**philos;
}	t_struct;

void	free_for_all(t_struct *box);
time_t	get_time(void);
void	append_time(time_t time);
int		ft_atoi(const char *nptr);
void	write_all(char *str, t_philo *philo);
int		box_init(t_struct *box, int argc, char **argv);
int		check_argv(int argc, char **argv);
void	check_and_exit(t_struct *box);
void	*check_die(void	*arg);
int		check_routine(t_philo *philo);
void	close_semaphore(t_struct *box);

#endif
