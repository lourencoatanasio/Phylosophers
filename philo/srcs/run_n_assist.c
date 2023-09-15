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
	if(check == 3)
	{
		if(((t_philo *) philo)->forks->left_fork->value == ((t_philo *) philo)->index)
		{
			pthread_mutex_unlock(&((t_philo *) philo)->forks->left_fork->mutex);
			((t_philo *) philo)->forks->left_fork->value = 0;
		}
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
		if (died == 1 || pickup_fork((t_philo *)philo, LEFT_FORK, &died) == 1)
			break ;
		if(((t_philo *)philo)->index % 2 != 0 && ((t_philo *) philo)->forks->right_fork != NULL && is_dead((t_philo *) philo, &died) != 1)
		{
			if (((t_philo *) philo)->forks->right_fork->value == 1 && is_dead((t_philo *) philo, &died) != 1)
			{
				unlock((t_philo *) philo, 1);
				((t_philo *) philo)->no_fork = 1;
			}
		}
		else
		{
			if (((t_philo *) philo)->forks->left_fork->value == 1 && is_dead((t_philo *) philo, &died) != 1)
			{
				unlock((t_philo *) philo, 1);
				((t_philo *) philo)->no_fork = 1;
			}
		}
		if(((t_philo *)philo)->no_fork == 0 && is_dead((t_philo *) philo, &died) != 1)
		{
			if(((t_philo *)philo)->index % 2 != 0)
				message((t_philo *)philo, LEFT_FORK, &died);
			else
				message((t_philo *)philo, RIGHT_FORK, &died);
			if (died == 1 || pickup_fork((t_philo *)philo, RIGHT_FORK, &died) == 1)
			{
				unlock((t_philo *) philo, 3);
				break ;
			}
			if (died == 1 || eat((t_philo *) philo, &died) == 1)
			{
				unlock((t_philo *) philo, 3);
				break ;
			}
			unlock((t_philo *) philo, 2);
			if (died == 1 || sleeping((t_philo *) philo, &died) == 1)
			{
				unlock((t_philo *) philo, 3);
				break ;
			}
			if (died == 1 || is_dead((t_philo *) philo, &died) == 1)
			{
				unlock((t_philo *) philo, 3);
				break ;
			}
			message((t_philo *) philo, THINK, &died);
		}
	}
	return (NULL);
}
