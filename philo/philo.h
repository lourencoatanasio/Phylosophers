//
// Created by ldiogo on 3/13/23.
//

#ifndef PHILO_PHILO_H
#define PHILO_PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_node
{
	pthread_mutex_t mutex;
	int value;
	int index;
	struct s_node *next;
} t_node;

typedef struct s_times
{
	int time_death;
	int time_sleep;
	int time_eat;
} t_times;

typedef struct s_forks
{
	t_node *left_fork;
	t_node *right_fork;
} t_forks;

typedef struct s_philo
{
	t_forks *forks;
	t_times *times;
	struct timeval start_time;
	long long int last_eat;
	int index;
	int num_philo;
	int *start;
} t_philo;

#endif //PHILO_PHILO_H
