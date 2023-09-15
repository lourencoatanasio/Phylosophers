/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_n_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldiogo <ldiogo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 11:49:41 by ldiogo            #+#    #+#             */
/*   Updated: 2023/04/12 11:49:42 by ldiogo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

long long int	s_to_mil(struct timeval t)
{
	long long int	res;

	res = t.tv_sec * 1000;
	res += t.tv_usec / 1000;
	return (res);
}

long long int	gt(struct timeval start)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000) - s_to_mil(start));
}

int	is_dead(t_philo *philo, int *died)
{
	if (gt(philo->start_time) - philo->last_eat > philo->times->time_death)
	{
		message(philo, DIED, died);
		unlock((t_philo *)philo, 3);
		*died = 1;
		return (1);
	}
	return (0);
}

int	eat(t_philo *philo, int *died)
{
	((t_philo *)philo)->last_eat = gt(((t_philo *)philo)->start_time);
	message(philo, EAT, died);
	philo->times_ate[philo->index - 1]++;
	while (gt(philo->start_time) - philo->last_eat < philo->times->time_eat)
	{
		if (*died == 1 || is_dead(philo, died) == 1)
			return (1);
	}
	return (0);
}

int	sleeping(t_philo *philo, int *died)
{
	message(philo, SLEEP, died);
	while (gt(philo->start_time) - philo->last_eat
		< philo->times->time_sleep + philo->times->time_eat)
	{
		if (is_dead(philo, died) == 1 || *died == 1)
			return (1);
	}
	return (0);
}
