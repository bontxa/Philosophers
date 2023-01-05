/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_init_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboncine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 14:55:49 by aboncine          #+#    #+#             */
/*   Updated: 2023/01/04 17:38:26 by aboncine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	write_all(char *str, t_philo *philo)
{
	sem_wait(philo->box->write);
	sem_wait(philo->box->death);
	if (philo->box->exit == 0)
		printf("%ld\t%d\t%s", get_time() - philo->box->init_time,
			philo->phi_id + 1, str);
	sem_post(philo->box->death);
	sem_post(philo->box->write);
}

static int	box_init2(t_struct *box)
{
	int	i;

	i = 0;
	sem_unlink("sem");
	sem_unlink("write");
	sem_unlink("eating");
	sem_unlink("death");
	box->sem = sem_open("sem", O_CREAT, 0644, box->philo);
	box->write = sem_open("write", O_CREAT, 0644, 1);
	box->eating = sem_open("eating", O_CREAT, 0644, 1);
	box->death = sem_open("death", O_CREAT, 0644, 1);
	while (i < box->philo)
	{
		box->philos[i] = (t_philo *) malloc(sizeof(t_philo));
		if (!box->philos[i])
			return (0);
		box->philos[i]->phi_id = i;
		box->philos[i]->box = box;
		box->philos[i]->eat_count = 0;
		box->philos[i]->start_eat = 0;
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
	box->init_time = get_time();
	box->exit = 0;
	box->all_ate = 0;
	if (box_init2(box) == 0)
		return (0);
	return (1);
}
