/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboncine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 08:59:58 by aboncine          #+#    #+#             */
/*   Updated: 2023/01/02 15:16:18 by aboncine         ###   ########.fr       */
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
	int			phi_id;
	int			eat_count;
	int			left;
	int			right;
	time_t		start_eat;
	pthread_t	thr_arr;
	t_struct	*box;
}	t_philo;

typedef struct s_struct {
	int				philo;
	int				exit;
	int				all_ate;
	int				times;
	int				argc;
	time_t			init_time;
	time_t			die;
	time_t			eat;
	time_t			sleep;
	pthread_mutex_t	*mut_arr;
	pthread_mutex_t	write;
	pthread_mutex_t	eating;
	t_philo			**philos;

}	t_struct;

void	free_for_all(t_struct *box);
void	*my_malloc(int size);
time_t	get_time(void);
void	append_time(time_t time);
int		ft_atoi(const char *nptr);
void	write_all(char *str, t_philo *philo);
int		box_init(t_struct *box, int argc, char **argv);
int		check_argv(int argc, char **argv);
void	check_and_exit(t_struct *box);
int		choose_fork_a(int left, int right);
int		choose_fork_b(int right, int left);
int		check_1(t_philo *philo);
int		check_loop(t_philo *philo);

#endif
