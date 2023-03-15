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
	int value;
	int index;
	struct s_node *next;
} t_node;

typedef struct s_thread
{
    pthread_t thread;
} t_thread;

typedef struct s_forks
{
	t_node *left_fork;
	t_node *right_fork;
} t_forks;

typedef struct s_philo
{
	t_forks *forks;
	struct timeval start_time;
	int index;
	int num_philo;
	int *start;
} t_philo;

#endif //PHILO_PHILO_H
