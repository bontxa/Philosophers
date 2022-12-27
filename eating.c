/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboncine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 16:18:04 by aboncine          #+#    #+#             */
/*   Updated: 2022/12/27 17:02:22 by aboncine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	lock_even(t_philo *philo)
{
	if (ID == philo->box->philo - 1)
		pthread_mutex_lock(MUTEX[0]);
	else
		pthread_mutex_lock(MUTEX[ID + 1]);
	printf("%ld\t%d\thas taken right fork\n", get_time() - INIT_TIME, ID + 1);
	pthread_mutex_lock(MUTEX[ID]);
	printf("%ld\t%d\thas taken left fork\n", get_time() - INIT_TIME, ID + 1);
}

static void	lock_odd(t_philo *philo)
{
	pthread_mutex_lock(MUTEX[ID]);
	printf("%ld\t%d\thas taken left fork\n", get_time() - INIT_TIME, ID + 1);
	if (ID == philo->box->philo - 1)
		pthread_mutex_lock(MUTEX[0]);
	else
		pthread_mutex_lock(MUTEX[ID + 1]);
	printf("%ld\t%d\thas taken right fork\n", get_time() - INIT_TIME, ID + 1);
}

static void	is_eating_unlimited(t_philo *philo)
{
	int	i;

	i = 0;
	while(i == 0)
	{
		if (ID % 2)
			lock_odd(philo);
		else
			lock_even(philo);
		philo->start_eat = get_time();
		printf("%ld\t%d\tis eating\n", get_time() - INIT_TIME, ID + 1);
		philo->eat_count++;
		append_time(philo->box->eat);
		pthread_mutex_unlock(MUTEX[ID]);
		if (ID == philo->box->philo - 1)
			pthread_mutex_unlock(MUTEX[0]);
		else
			pthread_mutex_unlock(MUTEX[ID + 1]);
		printf("%ld\t%d\tis sleeping\n", get_time() - INIT_TIME, ID + 1);
		append_time(philo->box->sleep);
		printf("%ld\t%d\tis thinking\n", get_time() - INIT_TIME, ID + 1);
	}
}

static void	is_eating_limited(t_philo *philo)
{
	int	i;

	i = 0;
	while(i < philo->box->times)
	{
		if (ID % 2)
			lock_odd(philo);
		else
			lock_even(philo);
		philo->start_eat = get_time();
		printf("%ld\t%d\tis eating\n", get_time() - INIT_TIME, ID + 1);
		philo->eat_count++;
		append_time(philo->box->eat);
		i++;
		pthread_mutex_unlock(MUTEX[ID]);
		if (ID == philo->box->philo - 1)
			pthread_mutex_unlock(MUTEX[0]);
		else
			pthread_mutex_unlock(MUTEX[ID + 1]);
		printf("%ld\t%d\tis sleeping\n", get_time() - INIT_TIME, ID + 1);
		append_time(philo->box->sleep);
		printf("%ld\t%d\tis thinking\n", get_time() - INIT_TIME, ID + 1);
	}
}

void	*is_eating(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->box->argc == 6)
		is_eating_limited(philo);
	else
		is_eating_unlimited(philo);
	return (NULL);
}
