/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboncine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 16:18:04 by aboncine          #+#    #+#             */
/*   Updated: 2023/01/02 15:14:19 by aboncine         ###   ########.fr       */
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

int	choose_fork_a(int left, int right)
{
	if (left < right)
		return (left);
	return (right);
}

int	choose_fork_b(int right, int left)
{
	if (right < left)
		return (left);
	return (right);
}

int	check_1(t_philo *philo)
{
	printf("%d\t%d\t%s", 0, 1, "has taken a fork\n");
	usleep(philo->box->eat);
	return (1);
}

int	check_loop(t_philo *philo)
{
	int		loop;

	pthread_mutex_lock(&philo->box->eating);
	loop = philo->box->exit;
	pthread_mutex_unlock(&philo->box->eating);
	return (loop);
}
