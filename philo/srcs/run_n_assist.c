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
	static pthread_mutex_t	mutex = PTHREAD_MUTEX_INITIALIZER;

	check_eat(philo, died);
	if (*died == 1)
		return ;
	pthread_mutex_lock(&mutex);
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
	pthread_mutex_unlock(&mutex);
}

void	wait(t_philo *philo, int *died)
{
	while (1)
		if (is_dead((t_philo *) philo, died) == 1)
			return ;
}

void	unlock_own(t_philo *p)
{
	if (((t_philo *) p)->forks->left_fork->value == ((t_philo *) p)->index)
	{
		pthread_mutex_unlock(&((t_philo *) p)->forks->left_fork->mutex);
		((t_philo *) p)->forks->left_fork->value = 0;
	}
}

void	unlock(t_philo *p, int check)
{
	if (check == 1)
	{
		if (((t_philo *)p)->index % 2 != 0)
		{
			((t_philo *) p)->forks->left_fork->value = 0;
			pthread_mutex_unlock(&((t_philo *) p)->forks->left_fork->mutex);
		}
		else
		{
			((t_philo *) p)->forks->right_fork->value = 0;
			pthread_mutex_unlock(&((t_philo *) p)->forks->right_fork->mutex);
		}
	}
	if (check == 2)
	{
		((t_philo *) p)->forks->right_fork->value = 0;
		((t_philo *) p)->forks->left_fork->value = 0;
		pthread_mutex_unlock(&((t_philo *) p)->forks->left_fork->mutex);
		pthread_mutex_unlock(&((t_philo *) p)->forks->right_fork->mutex);
	}
}

void	*philosopher(void *p)
{
	static int		died;

	died = 0;
	while (*((t_philo *)p)->start < ((t_philo *)p)->num_philo)
		;
	gettimeofday(&((t_philo *)p)->start_time, NULL);
	if (((t_philo *)p)->index % 2 == 0)
		usleep(10000);
	while (died == 1 || is_dead((t_philo *)p, &died) == 0)
	{
		((t_philo *)p)->no_fork = 0;
		if (died == 1 || pickup_fork((t_philo *)p, LEFT_FORK, &died) == 1)
		{
			unlock_own((t_philo *)p);
			break ;
		}
		if (((t_philo *)p)->no_fork == 0 && is_dead((t_philo *) p, &died) != 1)
		{
			filler_message((t_philo *)p, &died);
			if (life(p, &died) == 1)
				break ;
			message((t_philo *) p, THINK, &died);
		}
	}
	return (NULL);
}
