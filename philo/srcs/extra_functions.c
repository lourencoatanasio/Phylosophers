/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldiogo <ldiogo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 16:20:16 by ldiogo            #+#    #+#             */
/*   Updated: 2023/09/19 16:20:17 by ldiogo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	filler_message(t_philo *philo, int *died)
{
	if ((philo)->index % 2 != 0)
		message(philo, LEFT_FORK, died);
	else
		message(philo, RIGHT_FORK, died);
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
		while (((t_philo *)p)->forks->left_fork->value != 0)
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
		while (((t_philo *)p)->forks->right_fork->value != 0)
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
