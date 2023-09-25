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
	check_eat(philo, died);
	if (check_dead(philo, died) == 1)
		return ;
	pthread_mutex_lock(philo->write);
	if (message == 0)
		printf("\033[1;34m%lld %d has taken a left fork\n\033[0m",
			gt(((t_philo *) philo)->start_time), ((t_philo *) philo)->index);
	else if (message == 1)
		printf("\033[1;34m%lld %d has taken a right fork\n\033[0m",
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

int check_own_fork(t_philo *philo, int fork)
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

void	unlock_own(t_philo *p)
{
    pthread_mutex_lock(((t_philo *) p)->unlock);
    if (check_own_fork(((t_philo *)p), ((t_philo *)p)->forks->left_fork->value) == 1)
	{
		pthread_mutex_unlock(&((t_philo *) p)->forks->left_fork->mutex);
		((t_philo *) p)->forks->left_fork->value = 0;
	}
    if(check_own_fork(((t_philo *)p), ((t_philo *)p)->forks->right_fork->value) == 1)
    {
        pthread_mutex_unlock(&((t_philo *) p)->forks->right_fork->mutex);
        ((t_philo *) p)->forks->right_fork->value = 0;
    }
    pthread_mutex_unlock(((t_philo *) p)->unlock);
}

void	unlock(t_philo *p, int check)
{
	if (check == 1)
	{
		pthread_mutex_lock(((t_philo *) p)->forks_mutex);
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
		pthread_mutex_unlock(((t_philo *) p)->forks_mutex);
	}
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

int waiting(t_philo *p)
{
	pthread_mutex_lock(((t_philo *)p)->begin);
	if(*((t_philo *)p)->start < ((t_philo *)p)->num_philo)
	{
		pthread_mutex_unlock(((t_philo *)p)->begin);
		return (1);
	}
	else
	{
		pthread_mutex_unlock(((t_philo *)p)->begin);
		return (0);
	}
}

void	destroy_mutex(t_begin *begin)
{
	t_node *tmp;

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

void	*philosopher(void *p)
{
	static int		died;

	pthread_mutex_lock(((t_philo *)p)->death);
	died = 0;
	pthread_mutex_unlock(((t_philo *)p)->death);
	while (waiting((t_philo *)p) == 1)
		;
	usleep(1000);
	gettimeofday(&((t_philo *)p)->start_time, NULL);
	if (((t_philo *)p)->index % 2 == 0)
		usleep(10000);
	while (check_dead((t_philo *)p, &died) != 1 || is_dead((t_philo *)p, &died) == 0)
	{
		if (check_dead((t_philo *)p, &died) == 1 || pickup_fork((t_philo *)p, LEFT_FORK, &died) == 1)
		{
			unlock_own((t_philo *)p);
			break ;
		}
		if (is_dead((t_philo *) p, &died) != 1)
		{
			filler_message((t_philo *)p, &died);
			if (life(p, &died) == 1)
            {
                unlock_own((t_philo *)p);
                break ;
            }
			message((t_philo *) p, THINK, &died);
		}
	}
    unlock_own((t_philo *)p);
    printf("philo %d forks %d %d\n", ((t_philo *)p)->index, ((t_philo *)p)->forks->left_fork->value, ((t_philo *)p)->forks->right_fork->value);
    unlock_own((t_philo *)p);
	return (NULL);
}
