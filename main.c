/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboncine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 08:58:51 by aboncine          #+#    #+#             */
/*   Updated: 2022/12/27 17:02:24 by aboncine         ###   ########.fr       */
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

int	check_die(t_struct	*box)
{
	int			i;
	int			loop;

	i = 0;
	loop = 0;
	while(loop == 0)
	{
		while (i < box->philo)
		{
			if (box->argc == 6)
			{
				if (check_eat(box) == 0)
					return (1);
			}
			if ((get_time() - box->philos[i]->start_eat) > box->die)
			{
				printf("%ld %d died\n", DIED);
				return (1);
			}
			i++;
		}
		i = 0;
	}
	return (0);
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
	box->mut_arr = (pthread_mutex_t *) my_malloc(sizeof(pthread_mutex_t) * box->philo);
	box->init_time = get_time();
}

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

int main(int argc, char **argv)
{
	if (argc < 2 || argc > 6 || check_argv(argc, argv) == 1)
	{
		write(2, "Error\n", 6);
		return (0);
	}
	t_struct	*box;
	int	i;

	box = (t_struct *) my_malloc(sizeof(t_struct));
	i = 0;
	box_init(box, argc, argv);
	while (i < box->philo)
	{
		box->philos[i] = (t_philo *) my_malloc(sizeof(t_philo));
		box->philos[i]->phi_id = i;
		box->philos[i]->box = box;
		box->philos[i]->start_eat = box->init_time;
		box->philos[i]->eat_count = 0;
		pthread_mutex_init(&box->mut_arr[i], NULL);
		pthread_create(&box->philos[i]->thr_arr, NULL, &is_eating, box->philos[i]);
		i++;
	}
	if (check_die(box) == 1)
	{
		free_for_all(box);
		free(box);
		return (0);
	}
	i = 0;
	while (i < box->philo)
		pthread_join(box->philos[i++]->thr_arr, NULL);
	free_for_all(box);
	free(box);
}
