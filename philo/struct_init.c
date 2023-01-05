/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboncine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 14:55:49 by aboncine          #+#    #+#             */
/*   Updated: 2023/01/02 15:13:05 by aboncine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	box_init2(t_struct *box)
{
	int	i;

	i = 0;
	while (i < box->philo)
	{
		if (pthread_mutex_init(&box->mut_arr[i], NULL))
			return (0);
		box->philos[i] = (t_philo *) malloc(sizeof(t_philo));
		if (!box->philos[i])
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
	box->philos = (t_philo **) malloc(sizeof(t_philo *) * box->philo);
	if (!box->philos)
		return (0);
	box->mut_arr = (pthread_mutex_t *)
		malloc(sizeof(pthread_mutex_t) * box->philo);
	if (!box->mut_arr)
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
