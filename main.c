/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboncine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 08:58:51 by aboncine          #+#    #+#             */
/*   Updated: 2023/01/02 15:07:39 by aboncine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	lock_forks_and_eat(t_philo *philo)
{
	if (philo->left == philo->right)
		return (check_1(philo));
	pthread_mutex_lock
		(&philo->box->mut_arr[choose_fork_a(philo->left, philo->right)]);
	write_all("has taken a fork\n", philo);
	pthread_mutex_lock
		(&philo->box->mut_arr[choose_fork_b(philo->right, philo->left)]);
	write_all("has taken a fork\n", philo);
	write_all("is eating\n", philo);
	pthread_mutex_lock(&philo->box->eating);
	philo->start_eat = get_time();
	philo->eat_count++;
	pthread_mutex_unlock(&philo->box->eating);
	append_time(philo->box->eat);
	return (0);
}

static void	*is_eating(void *arg)
{
	t_philo	*philo;
	int		temp;

	philo = (t_philo *)arg;
	if (philo->phi_id % 2)
		usleep(15000);
	while (!(check_loop(philo)))
	{
		if ((lock_forks_and_eat(philo)) == 1)
			break ;
		pthread_mutex_unlock
			(&philo->box->mut_arr[choose_fork_b(philo->left, philo->right)]);
		pthread_mutex_unlock
			(&philo->box->mut_arr[choose_fork_a(philo->right, philo->left)]);
		pthread_mutex_lock(&philo->box->eating);
		temp = philo->box->all_ate;
		pthread_mutex_unlock(&philo->box->eating);
		if (temp)
			break ;
		write_all("is sleeping\n", philo);
		append_time(philo->box->sleep);
		write_all("is thinking\n", philo);
	}
	return (NULL);
}

static int	create_thread(t_struct *box, int i)
{
	if (pthread_create(&box->philos[i]->thr_arr, NULL,
			&is_eating, box->philos[i]))
	{
		while (i--)
			pthread_join (box->philos[i]->thr_arr, NULL);
		free_for_all(box);
		free (box);
		return (0);
	}
	pthread_mutex_lock(&box->eating);
	box->philos[i]->start_eat = get_time();
	pthread_mutex_unlock(&box->eating);
	return (1);
}

static int	first_check(int argc, char **argv)
{
	if (argc < 2 || argc > 6 || check_argv(argc, argv) == 1)
	{
		write(2, "Error\n", 6);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_struct	*box;
	int			i;

	if (first_check (argc, argv) == 0)
		return (-1);
	box = (t_struct *) malloc(sizeof(t_struct));
	if (!box)
		return (-1);
	i = 0;
	if (box_init(box, argc, argv) == 0)
		return (-1);
	while (i < box->philo)
	{
		if (create_thread(box, i) == 0)
			return (-1);
		i++;
	}
	check_and_exit(box);
	return (0);
}
