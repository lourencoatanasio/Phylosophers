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
		printf("\033[1;34m%lld ms philo %d took left fork\n\033[0m",
			gt(((t_philo *) philo)->start_time), ((t_philo *) philo)->index);
	else if (message == 1)
		printf("\033[1;34m%lld ms philo %d took right fork\n\033[0m",
			gt(((t_philo *) philo)->start_time), ((t_philo *) philo)->index);
	else if (message == 2)
		printf("\033[1;91m%lld ms philo %d is eating\n\033[0m",
			gt(((t_philo *) philo)->start_time), ((t_philo *) philo)->index);
	else if (message == 3)
		printf("\033[1;92m%lld ms philo %d is sleeping\n\033[0m",
			gt(((t_philo *) philo)->start_time), ((t_philo *) philo)->index);
	else if (message == 4)
		printf("\033[1;93m%lld ms philo %d is thinking\n\033[0m",
			gt(((t_philo *) philo)->start_time), ((t_philo *) philo)->index);
	else if (message == 5)
		printf("\033[1;30m%lld ms philo %d died\n\033[0m",
			gt(((t_philo *) philo)->start_time), ((t_philo *) philo)->index);
	pthread_mutex_unlock(&mutex);
}

void	wait(t_philo *philo, int *died)
{
	while (1)
		if (is_dead((t_philo *) philo, died) == 1)
			return ;
}

void	unlock(t_philo *philo, int check)
{
	if(check == 1)
	{
		if(((t_philo *)philo)->index % 2 != 0)
		{
			((t_philo *) philo)->forks->left_fork->value = 0;
			pthread_mutex_unlock(&((t_philo *) philo)->forks->left_fork->mutex);
		}
		else
		{
			((t_philo *) philo)->forks->right_fork->value = 0;
			pthread_mutex_unlock(&((t_philo *) philo)->forks->right_fork->mutex);
		}
	}
	if(check == 2)
	{
		((t_philo *) philo)->forks->right_fork->value = 0;
		((t_philo *) philo)->forks->left_fork->value = 0;
		pthread_mutex_unlock(&((t_philo *) philo)->forks->left_fork->mutex);
		pthread_mutex_unlock(&((t_philo *) philo)->forks->right_fork->mutex);
	}
}

void	*philosopher(void *philo)
{
	static int		died;

	died = 0;
	while (*((t_philo *)philo)->start < ((t_philo *)philo)->num_philo)
		;
	gettimeofday(&((t_philo *)philo)->start_time, NULL);
	if (((t_philo *)philo)->index % 2 == 0)
		usleep(10000);
	while (died == 1 || is_dead((t_philo *)philo, &died) == 0)
	{
		((t_philo *)philo)->no_fork = 0;
		if (died == 1 || is_dead((t_philo *) philo, &died) == 1)
			break ;
		pickup_fork((t_philo *)philo, LEFT_FORK, &died);
		if(((t_philo *)philo)->index % 2 != 0 && ((t_philo *) philo)->forks->right_fork != NULL)
		{
			if (((t_philo *) philo)->forks->right_fork->value == 1)
			{
				unlock((t_philo *) philo, 1);
				((t_philo *) philo)->no_fork = 1;
			}
		}
		else
		{
			if (((t_philo *) philo)->forks->left_fork->value == 1)
			{
				unlock((t_philo *) philo, 1);
				((t_philo *) philo)->no_fork = 1;
			}
		}
		if(((t_philo *)philo)->no_fork == 0)
		{
			message((t_philo *)philo, LEFT_FORK, &died);
			pickup_fork((t_philo *) philo, RIGHT_FORK, &died);
			if (died == 1 || eat((t_philo *) philo, &died) == 1)
				break;
			unlock((t_philo *) philo, 2);
			if (died == 1 || sleeping((t_philo *) philo, &died) == 1)
				break;
			if (died == 1 || is_dead((t_philo *) philo, &died) == 1)
				break;
			message((t_philo *) philo, THINK, &died);
		}
	}
	return (NULL);
}
