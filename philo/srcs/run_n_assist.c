/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_n_assist.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldiogo <ldiogo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 11:49:38 by ldiogo            #+#    #+#             */
/*   Updated: 2023/04/12 11:49:39 by ldiogo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	message(t_philo *philo, int message, int *died)
{
	pthread_mutex_lock(philo->write);
	if (check_dead(philo, died) == 1)
	{
		pthread_mutex_unlock(philo->write);
		return ;
	}
	if (message == 0)
		printf("\033[1;34m%lld %d has taken a fork\n\033[0m",
			gt(((t_philo *) philo)->start_time), ((t_philo *) philo)->index);
	else if (message == 1)
		printf("\033[1;34m%lld %d has taken a fork\n\033[0m",
			gt(((t_philo *) philo)->start_time), ((t_philo *) philo)->index);
	else if (message == 2)
		printf("\033[1;91m%lld %d is eating\n\033[0m",
			gt(((t_philo *) philo)->start_time), ((t_philo *) philo)->index);
	else if (message == 3)
		printf("\033[1;92m%lld %d is sleeping\n\033[0m",
			gt(((t_philo *) philo)->start_time), ((t_philo *) philo)->index);
	else if (message == 4)
		printf("\033[1;93m%lld %d is thinking\n\033[0m",
			gt(((t_philo *) philo)->start_time), ((t_philo *) philo)->index);
	else if (message == 5)
		printf("\033[1;30m%lld %d died\n\033[0m",
			gt(((t_philo *) philo)->start_time), ((t_philo *) philo)->index);
	pthread_mutex_unlock(philo->write);
}

void	wait(t_philo *philo, int *died)
{
	while (1)
		if (is_dead((t_philo *) philo, died) == 1)
			return ;
}

int	check_own_fork(t_philo *philo, int fork)
{
	pthread_mutex_lock(philo->forks_mutex);
	if (fork == philo->index)
	{
		pthread_mutex_unlock(philo->forks_mutex);
		return (1);
	}
	pthread_mutex_unlock(philo->forks_mutex);
	return (0);
}

int	check_start(t_philo *philo)
{
	int	i;

	pthread_mutex_lock(philo->death);
	i = *((t_philo *)philo)->start;
	pthread_mutex_unlock(philo->death);
	return (i);
}

void	*philosopher(void *p)
{
	static int				died;
	static struct timeval	time;

	pthread_mutex_lock(((t_philo *)p)->death);
	died = 0;
	gettimeofday(&time, NULL);
	*((t_philo *)p)->start += 1;
	pthread_mutex_unlock(((t_philo *)p)->death);
	while (check_start((t_philo *)p) < ((t_philo *)p)->num_philo)
		;
	((t_philo *)p)->start_time = time;
	if (((t_philo *)p)->index % 2 == 0)
		usleep(1000);
	while (check_dead((t_philo *)p, &died) != 1
		|| is_dead((t_philo *)p, &died) == 0)
	{
		if (life(p, &died) == 1)
			break ;
	}
	return (NULL);
}
