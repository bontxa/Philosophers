/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks_and_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboncine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 14:53:42 by aboncine          #+#    #+#             */
/*   Updated: 2023/01/03 16:59:29 by aboncine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	write_all(char *str, t_philo *philo)
{
	int	temp;

	pthread_mutex_lock(&philo->box->write);
	pthread_mutex_lock(&philo->box->eating);
	temp = philo->box->exit;
	pthread_mutex_unlock(&philo->box->eating);
	if (temp == 0)
		printf("%ld\t%d\t%s", get_time() - philo->box->init_time,
			philo->phi_id + 1, str);
	pthread_mutex_unlock(&philo->box->write);
}

static int	check_eat(t_struct *box)
{
	int			count;
	int			i;
	int			meal_count;

	count = 0;
	i = 0;
	while (i < box->philo)
	{
		pthread_mutex_lock(&box->eating);
		meal_count = box->philos[i]->eat_count;
		pthread_mutex_unlock(&box->eating);
		if (meal_count >= box->times)
			count++;
		i++;
	}
	if (count == box->philo)
		return (0);
	return (1);
}

static void	check_die2(t_struct *box, int i)
{
	time_t	temp;

	pthread_mutex_lock(&box->eating);
	temp = (get_time() - box->philos[i]->start_eat);
	pthread_mutex_unlock(&box->eating);
	if (temp > box->die)
	{
		write_all("died\n", box->philos[i]);
		pthread_mutex_lock(&box->eating);
		box->exit = 1;
		pthread_mutex_unlock(&box->eating);
	}
	usleep(100);
}

static void	check_die(t_struct	*box)
{
	int			i;

	while (box->all_ate == 0)
	{
		i = -1;
		while (++i < box->philo && box->exit == 0)
			check_die2(box, i);
		if (box->exit == 1)
			break ;
		if (box->argc == 6)
		{
			if (check_eat(box) == 0)
			{
				pthread_mutex_lock(&box->eating);
				box->all_ate = 1;
				pthread_mutex_unlock(&box->eating);
			}
		}
	}
}

void	check_and_exit(t_struct *box)
{
	int	i;

	i = -1;
	check_die(box);
	while (++i < box->philo)
		pthread_join(box->philos[i]->thr_arr, NULL);
	i = -1;
	while (++i < box->philo)
		pthread_mutex_destroy(&box->mut_arr[i]);
	pthread_mutex_destroy(&box->write);
	pthread_mutex_destroy(&box->eating);
	free_for_all(box);
	free(box);
}
