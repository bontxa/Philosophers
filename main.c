/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboncine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 08:58:51 by aboncine          #+#    #+#             */
/*   Updated: 2022/12/29 16:42:41 by aboncine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_eat(t_struct *box)
{
	int			count;
	int			i;

	count = 0;
	i = 0;
	while (i < box->philo)
	{
		if (box->philos[i]->eat_count == box->times)
			count++;
		i++;
	}
	if (count == box->philo)
		return (0);
	return (1);
}

void	check_die(t_struct	*box)
{
	int			i;

	while (box->all_ate == 0)
	{
		i = -1;
		while (++i < box->philo && box->exit == 0)
		{
			pthread_mutex_lock(&box->died);
			if ((get_time() - box->philos[i]->start_eat) > box->die)
			{
				write_all("died\n", box->philos[i]);
				box->exit = 1;
			}
			pthread_mutex_unlock(&box->died);
			usleep(100);
		}
		if (box->exit == 1)
			break ;
		if (box->argc == 6)
		{
			if (check_eat(box) == 0)
				box->all_ate = 1;
		}
	}
}

void	check_and_exit(t_struct *box)
{
	int	i;

	i = -1;
	check_die(box);
	while (++i < box->philo)
	{
		pthread_join(box->philos[i]->thr_arr, NULL);
		pthread_mutex_destroy(&box->mut_arr[i]);
	}
	pthread_mutex_destroy(&box->write);
	pthread_mutex_destroy(&box->died);
	free_for_all(box);
	free(box);
}

int	first_check(int argc, char **argv)
{
	if (argc < 2 || argc > 6 || check_argv(argc, argv) == 1)
	{
		write(2, "Error\n", 6);
		return (0);
	}
	if (ft_atoi(argv[1]) == 1)
	{
		printf("%d\t%d\t%s", 0, 1, "has taken a fork\n");
		usleep(ft_atoi(argv[2]));
		printf("%d\t%d\t%s", ft_atoi(argv[2]), 1, "died\n");
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_struct	*box;
	int			i;

	if (first_check (argc, argv) == 0)
		return (0);
	box = (t_struct *) my_malloc(sizeof(t_struct));
	i = 0;
	box_init(box, argc, argv);
	while (i < box->philo)
	{
		box->philos[i] = (t_philo *) my_malloc(sizeof(t_philo));
		box->philos[i]->phi_id = i;
		box->philos[i]->box = box;
		box->philos[i]->eat_count = 0;
		if (pthread_mutex_init(&box->mut_arr[i], NULL))
			return (-1);
		if (pthread_create(&box->philos[i]->thr_arr, NULL,
				&is_eating, box->philos[i]))
			return (-1);
		box->philos[i]->start_eat = get_time();
		i++;
	}
	check_and_exit(box);
	return (0);
}
