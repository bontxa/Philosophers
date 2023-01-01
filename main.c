/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboncine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 08:58:51 by aboncine          #+#    #+#             */
/*   Updated: 2022/12/30 16:30:56 by aboncine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


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
		return (-1);
	if (box = (t_struct *) my_malloc(sizeof(t_struct)) == NULL)
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
