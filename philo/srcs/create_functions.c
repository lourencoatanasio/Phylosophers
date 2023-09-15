/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldiogo <ldiogo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 11:49:30 by ldiogo            #+#    #+#             */
/*   Updated: 2023/04/12 11:49:31 by ldiogo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

t_philo	*create_philo(t_forks *utils, int index, int *start, t_begin *begin)
{
	int		i;
	t_philo	*philo;

	i = 0;
	while (i < begin->num_philo)
	{
		begin->times_ate[i] = 0;
		i++;
	}
	philo = (t_philo *)malloc(sizeof(t_philo));
	philo->times = begin->times;
	philo->forks = utils;
	philo->index = index;
	philo->forks_list = begin->forks_list;
	philo->num_philo = begin->num_philo;
	philo->start = start;
	philo->last_eat = 0;
	philo->times_ate = begin->times_ate;
	philo->no_fork = 0;
	return (philo);
}

t_forks	*assign_forks(t_node *forks, int index, int num_philo)
{
	t_forks	*utils;

	utils = (t_forks *)malloc(sizeof(t_forks));
	utils->left_fork = list_index(forks, index);
	if (num_philo == 1)
		utils->right_fork = NULL;
	else if (list_index(forks, index + 1) == NULL)
		utils->right_fork = list_index(forks, 1);
	else
		utils->right_fork = list_index(forks, index + 1);
	return (utils);
}

void	create_threads(t_begin *begin, int num_philo)
{
	t_forks					*utils;
	pthread_t				*th;
	t_philo					**philo;
	t_numbers				nu;

	nu.i = 0;
	philo = (t_philo **)malloc(sizeof(t_philo *) * num_philo);
	th = (pthread_t *)malloc(sizeof(pthread_t) * num_philo);
	while (nu.i < num_philo)
	{
		utils = assign_forks(begin->forks_list, nu.i + 1, num_philo);
		philo[nu.i] = create_philo(utils, nu.i + 1, &nu.i, begin);
		pthread_create(&th[nu.i], NULL, philosopher, philo[nu.i]);
		nu.i++;
	}
	nu.j = 0;
	while (nu.j < num_philo)
	{
		pthread_join(th[nu.j], NULL);
		nu.j++;
	}
	nu.j = 0;
	free_all(philo, num_philo, th);
}

int	pickup_fork(t_philo *philo, int fork, int *died)
{
	if (fork == 0)
	{
		if(((t_philo *)philo)->index % 2 != 0)
		{
			while(((t_philo *)philo)->forks->left_fork->value != 0)
			{
				if (is_dead((t_philo *)philo, died) == 1)
					return (1);
			}
			((t_philo *) philo)->forks->left_fork->value = ((t_philo *) philo)->index;
			pthread_mutex_lock(&((t_philo *) philo)->forks->left_fork->mutex);
		}
		else
		{
			while(((t_philo *)philo)->forks->right_fork->value != 0)
			{
				if (is_dead((t_philo *)philo, died) == 1)
					return (1);
			}
			pthread_mutex_lock(&((t_philo *) philo)->forks->right_fork->mutex);
			((t_philo *) philo)->forks->right_fork->value = ((t_philo *) philo)->index;
		}
	}
	else if (fork == 1)
	{
		if(((t_philo *)philo)->index % 2 == 0)
		{
			while(((t_philo *)philo)->forks->left_fork->value != 0)
			{
				if (is_dead((t_philo *)philo, died) == 1)
					return (1);
			}
			message((t_philo *)philo, LEFT_FORK, died);
			((t_philo *) philo)->forks->left_fork->value = ((t_philo *) philo)->index;
			pthread_mutex_lock(&((t_philo *) philo)->forks->left_fork->mutex);
		}
		else
		{
			while(((t_philo *)philo)->forks->right_fork->value != 0)
			{
				if (is_dead((t_philo *)philo, died) == 1)
					return (1);
			}
			((t_philo *) philo)->forks->right_fork->value = ((t_philo *) philo)->index;
			message((t_philo *)philo, RIGHT_FORK, died);
			pthread_mutex_lock(&((t_philo *) philo)->forks->right_fork->mutex);
		}
	}
	return 0;
}

t_times	*create_times(int time_death, int time_eat, int time_sleep)
{
	t_times	*times;

	times = (t_times *)malloc(sizeof(t_times));
	times->time_eat = time_eat;
	times->time_sleep = time_sleep;
	times->time_death = time_death;
	return (times);
}
