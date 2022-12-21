/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboncine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 08:58:51 by aboncine          #+#    #+#             */
/*   Updated: 2022/12/21 18:20:05 by aboncine         ###   ########.fr       */
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

void	*my_malloc(int size)
{
	void	*ptr;

	ptr = malloc(size);
	if(!ptr)
		return (NULL);
	return (ptr);
}

time_t	get_time(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
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

void	is_sleeping_and_thinking(int phi_id, time_t sleep_time, time_t init_time)
{
	printf("%ld\t%d\tis sleeping\n", get_time() - init_time, phi_id);
	usleep(sleep_time * 1000);
	printf("%ld\t%d\tis thinking\n", get_time() - init_time, phi_id);
}

void	lock_even(t_philo *philo)
{
	if (philo->phi_id == philo->box->philo - 1)
		pthread_mutex_lock(&philo->box->mut_arr[0]);
	else
		pthread_mutex_lock(&philo->box->mut_arr[philo->phi_id + 1]);
	printf("%ld\t%d\thas taken right fork\n", get_time() - philo->box->init_time, philo->phi_id);
	pthread_mutex_lock(&philo->box->mut_arr[philo->phi_id]);
	printf("%ld\t%d\thas taken left fork\n", get_time() - philo->box->init_time, philo->phi_id);
}

void	lock_odd(t_philo *philo)
{
	pthread_mutex_lock(&philo->box->mut_arr[philo->phi_id]);
	printf("%ld\t%d\thas taken left fork\n", get_time() - philo->box->init_time, philo->phi_id);
	if (philo->phi_id == philo->box->philo - 1)
		pthread_mutex_lock(&philo->box->mut_arr[0]);
	else
		pthread_mutex_lock(&philo->box->mut_arr[philo->phi_id + 1]);
	printf("%ld\t%d\thas taken right fork\n", get_time() - philo->box->init_time, philo->phi_id);
}

void	*is_eating(void *arg)
{
	int i;
	t_philo	*philo;

	i = 0;
	philo = (t_philo *)arg;
	while(i < philo->box->times)
	{
		if (philo->phi_id % 2)
			lock_odd(philo);
		else
			lock_even(philo);
		if ((get_time() - philo->start_eat) > philo->box->die)
		{
			printf("%ld\t%d\thas died\n", get_time() - philo->box->init_time, philo->phi_id);
			exit (-1);
		}
		philo->start_eat = get_time();
		usleep(philo->box->eat * 1000);
		i++;
		printf("%ld\t%d\tis eating\n", get_time() - philo->box->init_time, philo->phi_id);
		pthread_mutex_unlock(&philo->box->mut_arr[philo->phi_id]);
		if (philo->phi_id == philo->box->philo - 1)
			pthread_mutex_unlock(&philo->box->mut_arr[0]);
		else
			pthread_mutex_unlock(&philo->box->mut_arr[philo->phi_id + 1]);
		printf("%d\tha mollato le forchette\n", philo->phi_id);
		is_sleeping_and_thinking(philo->phi_id, philo->box->sleep, philo->box->init_time);
	}
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
	box->philos = (t_philo **) my_malloc(sizeof(t_philo *) * box->philo);
	box->mut_arr = (pthread_mutex_t *) my_malloc(sizeof(pthread_mutex_t) * box->philo);
	box->init_time = get_time();
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
	while (box->i < box->philo)
	{
		box->philos[box->i] = (t_philo *) my_malloc(sizeof(t_philo));
		box->philos[box->i]->phi_id = box->i;
		box->philos[box->i]->box = box;
		box->philos[box->i]->start_eat = box->init_time;
		pthread_mutex_init(&box->mut_arr[box->i], NULL);
		pthread_create(&box->philos[box->i]->thr_arr, NULL, &is_eating, (void *)box->philos[box->i]);
		box->i++;
	}
	box->i = 0;
	while (box->i < box->philo)
		pthread_join(box->philos[box->i++]->thr_arr, NULL);
	free_for_all(box);
	free(box);
}
