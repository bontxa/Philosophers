/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating_and_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboncine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 16:18:04 by aboncine          #+#    #+#             */
/*   Updated: 2022/12/30 16:31:00 by aboncine         ###   ########.fr       */
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

static int	box_init2(t_struct *box)
{	
	int i;
	
	i = 0;
	while (i < box->philo)
	{
		if (pthread_mutex_init(&box->mut_arr[i], NULL))
			return (0);
		if (box->philos[i] = (t_philo *) my_malloc(sizeof(t_philo)) == NULL)
			return (0);
		box->philos[i]->phi_id = i;
		box->philos[i]->box = box;
		box->philos[i]->eat_count = 0;
		box->philos[i]->start_eat = 0;
		box->philos[i]->left = i;
		box->philos[i]->right = (i + 1) % box->philo;
		i++;
	}
	return (1);
}

int	box_init(t_struct *box, int argc, char **argv)
{
	box->argc = argc;
	if (box->argc == 6)
		box->times = ft_atoi(argv[5]);
	box->philo = ft_atoi(argv[1]);
	box->die = ft_atoi(argv[2]);
	box->eat = ft_atoi(argv[3]);
	box->sleep = ft_atoi(argv[4]);
	if (box->philos = (t_philo **)
		my_malloc(sizeof(t_philo *) * box->philo) == NULL)
		return (0);
	if (box->mut_arr = (pthread_mutex_t *)
		my_malloc(sizeof(pthread_mutex_t) * box->philo) == NULL)
		return (0);
	box->init_time = get_time();
	box->exit = 0;
	box->all_ate = 0;
	if (box_init2(box) == 0)
		return (0);
	if (pthread_mutex_init(&box->write, NULL))
		return (0);
	if (pthread_mutex_init(&box->eating, NULL))
		return (0);
	return (1);
}

static void	lock_forks_and_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->box->mut_arr[philo->left]);
	write_all("has taken a fork\n", philo);
	pthread_mutex_lock(&philo->box->mut_arr[philo->right]);
	write_all("has taken a fork\n", philo);
	write_all("is eating\n", philo);
	pthread_mutex_lock(&philo->box->eating);
	philo->start_eat = get_time();
	philo->eat_count++;
	pthread_mutex_unlock(&philo->box->eating);
	append_time(philo->box->eat);
}

void	*is_eating(void *arg)
{
	t_philo	*philo;
	int		temp;

	philo = (t_philo *)arg;
	if (philo->phi_id % 2)
		usleep(15000);
	while (!(philo->box->exit))
	{
		lock_forks_and_eat(philo);
		pthread_mutex_unlock(&philo->box->mut_arr[philo->left]);
		pthread_mutex_unlock(&philo->box->mut_arr[philo->right]);
		pthread_mutex_lock(&philo->box->eating);
		temp = philo->box->all_ate;
		pthread_mutex_unlock(&philo->box->eating);
		if (temp)
			break;
		write_all("is sleeping\n", philo);
		append_time(philo->box->sleep);
		write_all("is thinking\n", philo);
	}
	return (NULL);
}
