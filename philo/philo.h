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

#define LEFT_FORK 0
#define RIGHT_FORK 1
#define EAT 2
#define SLEEP 3
#define THINK 4
#define DIED 5

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
	int times_eat;
} t_philo;

#endif //PHILO_PHILO_H
