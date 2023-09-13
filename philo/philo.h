/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldiogo <ldiogo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 11:49:10 by ldiogo            #+#    #+#             */
/*   Updated: 2023/04/12 11:49:28 by ldiogo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

# define LEFT_FORK 0
# define RIGHT_FORK 1
# define EAT 2
# define SLEEP 3
# define THINK 4
# define DIED 5

typedef struct s_node
{
	pthread_mutex_t	mutex;
	int				value;
	int				index;
	struct s_node	*next;
}	t_node;

typedef struct s_numbers
{
	int	index;
	int	num_philo;
	int	i;
	int	j;
}	t_numbers;

typedef struct s_times
{
	int	time_death;
	int	time_sleep;
	int	time_eat;
	int	times_must_eat;
}	t_times;

typedef struct s_forks
{
	t_node	*left_fork;
	t_node	*right_fork;
}	t_forks;

typedef struct s_begin
{
	t_node	*forks_list;
	t_times	*times;
	int		*times_ate;
	int		num_philo;
}	t_begin;

typedef struct s_philo
{
	t_forks			*forks;
	t_times			*times;
	t_node			*forks_list;
	struct timeval	start_time;
	long long int	last_eat;
	int				index;
	int				num_philo;
	int				*start;
	int				*times_ate;
	int				no_fork;
}	t_philo;

void			message(t_philo *philo, int message, int *died);
void			wait(t_philo *philo, int *died);
void			unlock(t_philo *philo, int check);
void			*philosopher(void *philo);
long long int	s_to_mil(struct timeval t);
long long int	gt(struct timeval start);
int				is_dead(t_philo *philo, int *died);
int				eat(t_philo *philo, int *died);
int				sleeping(t_philo *philo, int *died);
int				ft_atoi(char *str);
t_node			*list_index(t_node *head, int index);
void			free_list(t_node **head);
t_node			*create_node(int value, int index);
void			add_node(t_node **head, t_node *node);
t_philo			*create_philo(t_forks *a, int b, int *c, t_begin *d);
t_forks			*assign_forks(t_node *forks, int index, int num_philo);
void			create_threads(t_begin *begin, int num_philo);
void			pickup_fork(t_philo *philo, int fork, int *died);
t_times			*create_times(int time_death, int time_eat, int time_sleep);
void			check_eat(t_philo *philo, int *died);
int				check_num(char *str);
int				check_args(int ac, char **av);
void			free_all(t_philo **philo, int num_philo, pthread_t	*th);
void			other_free(t_begin *begin);


#endif //PHILO_PHILO_H
