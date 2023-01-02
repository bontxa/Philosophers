/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboncine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 15:56:58 by aboncine          #+#    #+#             */
/*   Updated: 2023/01/02 15:14:42 by aboncine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_for_all(t_struct *box)
{
	int	i;

	i = 0;
	while (i < box->philo)
		free(box->philos[i++]);
	free(box->philos);
	free(box->mut_arr);
}

time_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	append_time(time_t time)
{
	time_t	tmp;

	tmp = get_time() + time;
	while (get_time() < tmp)
		usleep(50);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	nbr;

	i = 0;
	nbr = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '+')
		i++;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		nbr = nbr * 10 + nptr[i] - 48;
		i++;
	}
	return (nbr);
}
