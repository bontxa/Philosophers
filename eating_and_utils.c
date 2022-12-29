/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating_and_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboncine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 16:18:04 by aboncine          #+#    #+#             */
/*   Updated: 2022/12/29 16:42:36 by aboncine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_argv(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (i < argc)
	{
		if (argv[i][0] == '\0')
			return (1);
		if (argv[i][0] == '+' && argv[i][1] >= '0' && argv[i][1] <= '9')
			j++;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (1);
			j++;
		}
		j = 0;
		i++;
	}
	return (0);
}

void	write_all(char *str, t_philo *philo)
{
	pthread_mutex_lock(&philo->box->write);
	if (philo->box->exit == 0)
		printf("%ld\t%d\t%s", get_time() - philo->box->init_time,
			philo->phi_id + 1, str);
	pthread_mutex_unlock(&philo->box->write);
}

void	box_init(t_struct *box, int argc, char **argv)
{
	box->argc = argc;
	if (box->argc == 6)
		box->times = ft_atoi(argv[5]);
	box->philo = ft_atoi(argv[1]);
	box->die = ft_atoi(argv[2]);
	box->eat = ft_atoi(argv[3]);
	box->sleep = ft_atoi(argv[4]);
	box->philos = (t_philo **) my_malloc(sizeof(t_philo *) * box->philo);
	box->mut_arr = (pthread_mutex_t *)
		my_malloc(sizeof(pthread_mutex_t) * box->philo);
	box->init_time = get_time();
	box->exit = 0;
	box->all_ate = 0;
	if (pthread_mutex_init(&box->write, NULL))
		return ;
	if (pthread_mutex_init(&box->died, NULL))
		return ;
}

static void	lock_forks_and_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->box->mut_arr[philo->phi_id]);
	write_all("has taken a fork\n", philo);
	if (philo->phi_id == philo->box->philo - 1)
	{
		pthread_mutex_lock(&philo->box->mut_arr[0]);
		write_all("has taken a fork\n", philo);
	}
	else
	{
		pthread_mutex_lock(&philo->box->mut_arr[philo->phi_id + 1]);
		write_all("has taken a fork\n", philo);
	}
	pthread_mutex_lock(&(philo->box->died));
	write_all("is eating\n", philo);
	philo->start_eat = get_time();
	pthread_mutex_unlock(&philo->box->died);
	append_time(philo->box->eat);
	philo->eat_count++;
}

void	*is_eating(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->phi_id % 2)
		usleep(15000);
	while (!(philo->box->exit))
	{
		lock_forks_and_eat(philo);
		pthread_mutex_unlock(&philo->box->mut_arr[philo->phi_id]);
		if (philo->phi_id == philo->box->philo - 1)
			pthread_mutex_unlock(&philo->box->mut_arr[0]);
		else
			pthread_mutex_unlock(&philo->box->mut_arr[philo->phi_id + 1]);
		if (philo->box->all_ate)
			break ;
		write_all("is sleeping\n", philo);
		append_time(philo->box->sleep);
		write_all("is thinking\n", philo);
	}
	return (NULL);
}
