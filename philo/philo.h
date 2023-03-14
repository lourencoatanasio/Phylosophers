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
    int index;
} t_thread;

typedef struct s_forks
{
	t_node *left_fork;
	t_node *right_fork;
} t_forks;

#endif //PHILO_PHILO_H
