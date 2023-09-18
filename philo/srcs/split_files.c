/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldiogo <ldiogo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 12:53:47 by ldiogo            #+#    #+#             */
/*   Updated: 2023/09/18 12:53:48 by ldiogo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	life(t_philo *philo, int *died)
{
	if (*died == 1 || pickup_fork((t_philo *)philo, RIGHT_FORK, died) == 1)
	{
		unlock_own((t_philo *) philo);
		return (1);
	}
	if (*died == 1 || eat((t_philo *) philo, died) == 1)
	{
		unlock_own((t_philo *) philo);
		return (1);
	}
	unlock((t_philo *) philo, 2);
	if (*died == 1 || sleeping((t_philo *) philo, died) == 1)
	{
		unlock_own((t_philo *) philo);
		return (1);
	}
	if (*died == 1 || is_dead((t_philo *) philo, died) == 1)
	{
		unlock_own((t_philo *) philo);
		return (1);
	}
	return (0);
}

int	one_philo(char **av)
{
	if (ft_atoi(av[1]) == 1)
	{
		printf("\033[1;34m%i ms philo %d took a fork\n\033[0m", 0, 1);
		usleep(ft_atoi(av[2]) * 1000);
		printf("\033[1;30m%i ms philo %d died\n\033[0m", ft_atoi(av[2]) + 1, 1);
		return (0);
	}
	return (1);
}

int	fork_one(t_philo *p, int *died)
{
	if (((t_philo *)p)->index % 2 != 0)
	{
		while (is_dead((t_philo *)p, died) != 1
			&& ((t_philo *)p)->forks->left_fork->value != 0)
			;
		if (is_dead((t_philo *)p, died) == 1)
			return (1);
		((t_philo *) p)->forks->left_fork->value = ((t_philo *) p)->index;
		pthread_mutex_lock(&((t_philo *) p)->forks->left_fork->mutex);
	}
	else
	{
		while (is_dead((t_philo *)p, died) != 1
			&& ((t_philo *)p)->forks->right_fork->value != 0)
			;
		if (is_dead((t_philo *)p, died) == 1)
			return (1);
		((t_philo *) p)->forks->right_fork->value = ((t_philo *) p)->index;
		pthread_mutex_lock(&((t_philo *) p)->forks->right_fork->mutex);
	}
	return (0);
}

int	fork_two(t_philo *p, int *died)
{
	if (((t_philo *)p)->index % 2 == 0)
	{
		while (is_dead((t_philo *)p, died) != 1
			&& ((t_philo *)p)->forks->left_fork->value != 0)
		{
			if (is_dead((t_philo *)p, died) == 1)
				return (1);
		}
		message((t_philo *)p, LEFT_FORK, died);
		((t_philo *) p)->forks->left_fork->value = ((t_philo *) p)->index;
		pthread_mutex_lock(&((t_philo *) p)->forks->left_fork->mutex);
	}
	else
	{
		while (is_dead((t_philo *)p, died) != 1
			&& ((t_philo *)p)->forks->right_fork->value != 0)
		{
			if (is_dead((t_philo *)p, died) == 1)
				return (1);
		}
		((t_philo *) p)->forks->right_fork->value = ((t_philo *) p)->index;
		message((t_philo *)p, RIGHT_FORK, died);
		pthread_mutex_lock(&((t_philo *) p)->forks->right_fork->mutex);
	}
	return (0);
}
