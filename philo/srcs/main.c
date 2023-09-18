/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldiogo <ldiogo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 11:49:35 by ldiogo            #+#    #+#             */
/*   Updated: 2023/04/12 11:49:36 by ldiogo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	filler_message(t_philo *philo, int *died)
{
	if (((t_philo *)philo)->index % 2 != 0)
		message((t_philo *)philo, LEFT_FORK, died);
	else
		message((t_philo *)philo, RIGHT_FORK, died);
}

int	main(int ac, char **av)
{
	t_begin	*begin;
	int		i;

	if (check_args(ac, av) == 1)
		return (1);
	begin = (t_begin *)malloc(sizeof(t_begin));
	begin->num_philo = ft_atoi(av[1]);
	begin->times = create_times(ft_atoi(av[2]), ft_atoi(av[3]), ft_atoi(av[4]));
	begin->forks_list = NULL;
	i = 0;
	while (i < ft_atoi(av[1]))
	{
		add_node(&begin->forks_list, create_node(0, i + 1));
		i++;
	}
	begin->times_ate = (int *)malloc(sizeof(int) * ft_atoi(av[1]));
	if (ac == 6)
		begin->times->times_must_eat = ft_atoi(av[5]);
	else
		begin->times->times_must_eat = -1;
	create_threads(begin, ft_atoi(av[1]));
	free_list(&begin->forks_list);
	other_free(begin);
	return (0);
}
