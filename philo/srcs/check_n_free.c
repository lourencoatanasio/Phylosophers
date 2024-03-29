/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_n_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldiogo <ldiogo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 11:49:23 by ldiogo            #+#    #+#             */
/*   Updated: 2023/04/12 11:49:28 by ldiogo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	check_eat(t_philo *philo, int *died)
{
	int	i;

	i = 0;
	if (philo->times->times_must_eat == -1)
		return ;
	while (i < philo->num_philo)
	{
		if (philo->times_ate[i] < philo->times->times_must_eat)
			return ;
		i++;
	}
	pthread_mutex_lock(philo->death);
	*died = 1;
	pthread_mutex_unlock(philo->death);
}

int	check_num(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

int	check_args(int ac, char **av)
{
	int	i;

	if (ac != 6 && ac != 5)
	{
		printf("||Error: Wrong number of arguments||\n");
		return (1);
	}
	i = 1;
	while (i < ac)
	{
		if (check_num(av[i]) == 1)
		{
			printf("||Error: invalid argument||\n");
			return (1);
		}
		i++;
	}
	if (one_philo(av) == 0)
		return (1);
	return (0);
}

void	free_all(t_philo **philo, int num_philo, pthread_t	*th)
{
	int	j;

	j = 0;
	while (j < num_philo)
	{
		free(philo[j]->forks);
		free(philo[j]);
		j++;
	}
	free(th);
	free(philo);
}

void	other_free(t_begin *begin)
{
	free(begin->times);
	free(begin->times_ate);
	free(begin);
}
