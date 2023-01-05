/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks_and_utils_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboncine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 14:53:42 by aboncine          #+#    #+#             */
/*   Updated: 2023/01/04 17:43:33 by aboncine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	check_routine(t_philo *philo)
{
	int	temp;

	sem_wait(philo->box->eating);
	temp = philo->box->exit;
	sem_post(philo->box->eating);
	return (temp);
}

void	close_semaphore(t_struct *box)
{
	sem_close(box->sem);
	sem_close(box->write);
	sem_close(box->eating);
	sem_close(box->death);
	sem_unlink("sem");
	sem_unlink("write");
	sem_unlink("eating");
	sem_unlink("death");
}

static void	exit_die(t_philo *philo)
{
	write_all("died\n", philo);
	sem_wait(philo->box->death);
	philo->box->exit = 1;
	close_semaphore(philo->box);
	free_for_all(philo->box);
	exit (1);
}

void	*check_die(void	*arg)
{
	t_philo		*philo;

	philo = (t_philo *) arg;
	while (1)
	{
		sem_wait(philo->box->eating);
		if ((get_time() - philo->start_eat) > philo->box->die)
			exit_die(philo);
		sem_post(philo->box->eating);
		if (philo->box->exit == 1)
			break ;
		sem_wait(philo->box->eating);
		if (philo->eat_count >= philo->box->times && philo->box->argc == 6)
		{
			sem_post(philo->box->eating);
			break ;
		}
		sem_post(philo->box->eating);
	}
	return (NULL);
}

void	check_and_exit(t_struct *box)
{
	int	i;
	int	ret;

	i = -1;
	while (++i < box->philo)
	{
		waitpid(-1, &ret, 0);
		if (ret != 0)
		{
			i = 0;
			while (++i < box->philo)
				kill(box->philos[i]->pid, 9);
			break ;
		}
	}
	sem_close(box->sem);
	sem_close(box->write);
	sem_close(box->eating);
	sem_close(box->death);
	sem_unlink("sem");
	sem_unlink("write");
	sem_unlink("eating");
	sem_unlink("death");
	free_for_all(box);
}
