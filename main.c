/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboncine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 08:58:51 by aboncine          #+#    #+#             */
/*   Updated: 2022/12/19 17:42:19 by aboncine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*my_malloc(int size)
{
	void	*ptr;

	ptr = malloc(size);
	if(!ptr)
		return (NULL);
	return (ptr);
}

int	ft_atoi(const char *nptr)
{
	int		i;
	int		nbr;

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

void	*take_fork(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->box->mut_arr[philo->phi_id]);
	if (philo->phi_id == philo->box->philo - 1)
		pthread_mutex_lock(&philo->box->mut_arr[0]);
	else
		pthread_mutex_lock(&philo->box->mut_arr[philo->phi_id + 1]);
	struct timeval	time;
	gettimeofday(&time, NULL);
	printf("%ld\t%d\thas taken a fork\n", time.tv_sec * 1000000 + time.tv_usec, philo->phi_id);
	printf("%ld\t%d\thas taken a fork\n", time.tv_sec * 1000000 + time.tv_usec, philo->phi_id);
	printf("%ld\t%d\tis eating\n", time.tv_sec * 1000000 + time.tv_usec, philo->phi_id);
	sleep (1);
	pthread_mutex_unlock(&philo->box->mut_arr[philo->phi_id]);
	if (philo->phi_id == philo->box->philo - 1)
		pthread_mutex_unlock(&philo->box->mut_arr[0]);
	else
		pthread_mutex_unlock(&philo->box->mut_arr[philo->phi_id + 1]);
	return (NULL);
}

void	print_error(void)
{
	write(2, "Error\n", 6);
	exit(-1);
}

void	box_init(t_struct *box, char **argv)
{
	box->philo = ft_atoi(argv[1]);
	box->die = ft_atoi(argv[2]);
	box->eat = ft_atoi(argv[3]);
	box->sleep = ft_atoi(argv[4]);
	box->times = ft_atoi(argv[5]);
	box->i = 0;
}

static void	check_argv(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (i < argc)
	{
		if (argv[i][0] == '\0')
			print_error();
		if (argv[i][0] == '+' && argv[i][1] >= '0' && argv[i][1] <= '9')
			j++;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				print_error();
			j++;
		}
		j = 0;
		i++;
	}
}

int main(int argc, char **argv)
{
	if (argc < 2 && argc > 6)
		print_error();
	check_argv(argc, argv);
	t_struct	*box;
	box = (t_struct *) my_malloc(sizeof(t_struct));
	box_init(box, argv);
	box->philos = (t_philo **) my_malloc(sizeof(t_philo *) * box->philo);
	box->mut_arr = (pthread_mutex_t *) my_malloc(sizeof(pthread_mutex_t) * box->philo);
	while (box->i < box->philo)
	{
		box->philos[box->i] = (t_philo *) my_malloc(sizeof(t_philo));
		box->philos[box->i]->phi_id = box->i;
		box->philos[box->i]->box = box;
		pthread_mutex_init(&box->mut_arr[box->i], NULL);
		pthread_create(&box->philos[box->i]->thr_arr, NULL, &take_fork, (void *)box->philos[box->i]);
		box->i++;
	}
	box->i = 0;
	while (box->i < box->philo)
		pthread_join(box->philos[box->i++]->thr_arr, NULL);
}
