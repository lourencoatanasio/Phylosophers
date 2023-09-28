/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_split_files.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldiogo <ldiogo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 14:09:52 by ldiogo            #+#    #+#             */
/*   Updated: 2023/09/26 14:09:53 by ldiogo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	waiting(t_philo *p)
{
//	pthread_mutex_lock(((t_philo *)p)->begin);
//	printf("start: %d\n", *((t_philo *)p)->start);
	if (*((t_philo *)p)->start < ((t_philo *)p)->num_philo)
	{
//		pthread_mutex_unlock(((t_philo *)p)->begin);
		return (1);
	}
	else
	{
//		pthread_mutex_unlock(((t_philo *)p)->begin);
		return (0);
	}
}

void	destroy_mutex(t_begin *begin)
{
	t_node	*tmp;

	tmp = begin->forks_list;
	while (tmp)
	{
		pthread_mutex_destroy(&tmp->mutex);
		tmp = tmp->next;
	}
	pthread_mutex_destroy(&begin->write);
	pthread_mutex_destroy(&begin->death);
	pthread_mutex_destroy(&begin->begin);
	pthread_mutex_destroy(&begin->forks);
	pthread_mutex_destroy(&begin->unlock);
}

void	unlock(t_philo *p, int check)
{
	if (check == 2)
	{
		pthread_mutex_lock(((t_philo *) p)->forks_mutex);
		((t_philo *) p)->forks->right_fork->value = 0;
		((t_philo *) p)->forks->left_fork->value = 0;
		pthread_mutex_unlock(((t_philo *) p)->forks_mutex);
		pthread_mutex_unlock(&((t_philo *) p)->forks->left_fork->mutex);
		pthread_mutex_unlock(&((t_philo *) p)->forks->right_fork->mutex);
	}
}

int	check_dead(t_philo *philo, int *died)
{
	pthread_mutex_lock(philo->death);
	if (*died == 1)
	{
		pthread_mutex_unlock(philo->death);
		return (1);
	}
	pthread_mutex_unlock(philo->death);
	return (0);
}
