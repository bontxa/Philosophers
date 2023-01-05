/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboncine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 15:26:44 by aboncine          #+#    #+#             */
/*   Updated: 2023/01/04 17:44:02 by aboncine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

#include "philosophers_bonus.h"

static void	lock_forks_and_eat(t_philo *philo)
{
	sem_wait(philo->box->sem);
	write_all("has taken a fork\n", philo);
	sem_wait(philo->box->sem);
	write_all("has taken a fork\n", philo);
	write_all("is eating\n", philo);
	sem_wait(philo->box->eating);
	philo->start_eat = get_time();
	philo->eat_count++;
	sem_post(philo->box->eating);
	append_time(philo->box->eat);
	sem_post(philo->box->sem);
	sem_post(philo->box->sem);
}

static void	is_eating(t_philo *philo)
{
	philo->start_eat = get_time();
	pthread_create(&philo->check, NULL, check_die, philo);
	if (philo->phi_id % 2)
		usleep(15000);
	while (check_routine(philo) == 0)
	{
		lock_forks_and_eat(philo);
		sem_wait(philo->box->eating);
		if (philo->eat_count >= philo->box->times && philo->box->argc == 6)
		{
			sem_post(philo->box->eating);
			break ;
		}
		sem_post(philo->box->eating);
		write_all("is sleeping\n", philo);
		append_time(philo->box->sleep);
		write_all("is thinking\n", philo);
	}
	pthread_join(philo->check, NULL);
	close_semaphore(philo->box);
	free_for_all(philo->box);
	exit(1);
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

static int	philo_1(char **argv)
{
	printf("%d\t%d\t%s", 0, 1, "has taken a fork\n");
	usleep(ft_atoi(argv[2]));
	printf("%d\t%d\t%s", ft_atoi(argv[2]), 1, "died\n");
	return (0);
}

int	main(int argc, char **argv)
{
	t_struct	box;
	int			i;

	if (first_check (argc, argv) == 0)
		return (-1);
	if (ft_atoi(argv[1]) == 1)
		return (philo_1(argv));
	i = -1;
	if (box_init(&box, argc, argv) == 0)
		return (-1);
	while (++i < box.philo)
	{
		box.philos[i]->pid = fork();
		if (box.philos[i]->pid == 0)
			is_eating(box.philos[i]);
		usleep(100);
	}
	check_and_exit(&box);
	return (0);
}
