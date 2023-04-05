#include "philo.h"

int	ft_atoi(char *str)
{
	int		i;
	int		sign;
	int		result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
		   || str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
	{
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

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

t_node  *list_index(t_node *head, int index)
{
	t_node	*tmp;

	tmp = head;
	while (tmp != NULL)
	{
		if (tmp->index == index)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	free_list(t_node **head)
{
	t_node	*tmp;
	t_node	*tmp2;

	tmp = *head;
	while (tmp != NULL)
	{
		tmp2 = tmp->next;
		free(tmp);
		tmp = tmp2;
	}
	*head = NULL;
}

t_node	*create_node(int value, int index)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	node->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	node->value = value;
	node->index = index;
	node->next = NULL;
	return (node);
}

void	add_node(t_node **head, t_node *node)
{
	t_node	*tmp;

	if (*head == NULL)
	{
		*head = node;
		return ;
	}
	tmp = *head;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = node;
}

void	print_list(t_node *head)
{
	t_node	*tmp;

	tmp = head;
	while (tmp != NULL)
	{
		printf("index %d = %d\n", tmp->index, tmp->value);
		tmp = tmp->next;
	}
}

void	check_eat(t_philo *philo, int *died)
{
	int	i;

	i = 0;
	if(philo->times->times_must_eat == -1)
		return ;
	while (i < philo->num_philo)
	{
		if (philo->times_ate[i] < philo->times->times_must_eat)
			return ;
		i++;
	}
	*died = 1;
}

void message(t_philo *philo, int message, int *died)
{
	static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	check_eat(philo, died);
	if (*died == 1)
		return ;
	pthread_mutex_lock(&mutex);
	if(message == 0)
		printf("\033[1;34m%lld ms philo %d took left fork\n\033[0m", gt(((t_philo *) philo)->start_time), ((t_philo *) philo)->index);
	else if(message == 1)
		printf("\033[1;34m%lld ms philo %d took right fork\n\033[0m", gt(((t_philo *) philo)->start_time), ((t_philo *) philo)->index);
	else if(message == 2)
		printf("\033[1;91m%lld ms philo %d is eating\n\033[0m", gt(((t_philo *) philo)->start_time), ((t_philo *) philo)->index);
	else if(message == 3)
		printf("\033[1;92m%lld ms philo %d is sleeping\n\033[0m", gt(((t_philo *) philo)->start_time), ((t_philo *) philo)->index);
	else if(message == 4)
		printf("\033[1;93m%lld ms philo %d is thinking\n\033[0m", gt(((t_philo *) philo)->start_time), ((t_philo *) philo)->index);
	else if(message == 5)
		printf("\033[1;30m%lld ms philo %d died\n\033[0m", gt(((t_philo *) philo)->start_time), ((t_philo *) philo)->index);
	pthread_mutex_unlock(&mutex);
}


int is_dead(t_philo *philo, int *died)
{
	if (gt(philo->start_time) - philo->last_eat > philo->times->time_death)
	{
		message(philo, DIED, died);
		while (philo->forks_list != NULL)
		{
			pthread_mutex_unlock(&philo->forks_list->mutex);
			philo->forks_list = philo->forks_list->next;
		}
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
	while(gt(philo->start_time) - philo->last_eat < philo->times->time_eat)
	{
		if (*died == 1 || is_dead(philo, died) == 1)
			return 1;
	}
	return 0;
}

int sleeping(t_philo *philo, int *died)
{
	message(philo, SLEEP, died);
	while(gt(philo->start_time) - philo->last_eat < philo->times->time_sleep + philo->times->time_eat)
	{
		if (is_dead(philo, died) == 1 || *died == 1)
			return 1;
	}
	return 0;
}

void	*philosopher(void *philo)
{
	static int		died;

	died = 0;
//	((t_philo *) philo)->times_must_eat = 0;
    while (*((t_philo *)philo)->start < ((t_philo *)philo)->num_philo)
		;
	gettimeofday(&((t_philo *)philo)->start_time, NULL);
	if(((t_philo *)philo)->index % 2 == 0)
		usleep(60000);
	while(died == 1 || is_dead((t_philo *)philo, &died) == 0)
	{
		pthread_mutex_lock(&((t_philo *) philo)->forks->left_fork->mutex);
		message((t_philo *)philo, LEFT_FORK, &died);
		if (((t_philo *)philo)->forks->right_fork == NULL)
		{
			while (1)
			{
				if (is_dead((t_philo *) philo, &died) == 1)
					break;
			}
			break;
		}
		if (died == 1 || is_dead((t_philo *) philo, &died) == 1)
			break ;
		pthread_mutex_lock(&((t_philo *) philo)->forks->right_fork->mutex);
		message((t_philo *)philo, RIGHT_FORK , &died);
		if (died == 1 || eat((t_philo *) philo, &died) == 1)
			break ;
		pthread_mutex_unlock(&((t_philo *) philo)->forks->left_fork->mutex);
		pthread_mutex_unlock(&((t_philo *) philo)->forks->right_fork->mutex);
		if (died == 1 || sleeping((t_philo *) philo, &died) == 1)
			break;
		if (died == 1 || is_dead((t_philo *) philo, &died) == 1)
			break;
		message((t_philo *)philo, THINK , &died);
	}
}

t_philo *create_philo(t_forks *utils, int index, int num_philo, int *start, t_begin *begin)
{
	int i;
	t_philo *philo;

	i = 0;
	while(i < num_philo)
	{
		begin->times_ate[i] = 0;
		i++;
	}
	philo = (t_philo *)malloc(sizeof(t_philo));
	philo->times = begin->times;
	philo->forks = utils;
	philo->index = index;
	philo->forks_list = begin->forks_list;
	philo->num_philo = num_philo;
	philo->start = start;
	philo->last_eat = 0;
	philo->times_ate = begin->times_ate;
	return (philo);
}

t_forks *assign_forks(t_node *forks, int index, int num_philo)
{
	t_forks *utils;
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

void    create_threads(t_begin *begin, int num_philo)
{
    int i;
	int j;
    t_forks *utils;
    pthread_t *th;
	t_philo **philo;

    i = 0;
	philo = (t_philo **)malloc(sizeof(t_philo *) * num_philo);
    th = (pthread_t *)malloc(sizeof(pthread_t) * num_philo);
    while (i < num_philo)
    {
        utils = assign_forks(begin->forks_list, i + 1, num_philo);
		philo[i] = create_philo(utils, i + 1, num_philo, &i, begin);
        pthread_create(&th[i], NULL, philosopher, philo[i]);
        i++;
    }
    j = 0;
    while (j < num_philo)
    {
        pthread_join(th[j], NULL);
        j++;
    }
	j = 0;
	while(j < num_philo)
	{
		free(philo[j]->forks);
		free(philo[j]);
		j++;
	}
	free(philo);
	free(th);
}

t_times *create_times(int time_death, int time_eat, int time_sleep)
{
	t_times *times;
	times = (t_times *)malloc(sizeof(t_times));
	times->time_eat = time_eat;
	times->time_sleep = time_sleep;
	times->time_death = time_death;
	return (times);
}

int check_num(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return 1;
		i++;
	}
	return 0;
}

int main(int ac, char **av)
{
	t_begin *begin;
	int i;

	if (ac != 6 && ac != 5)
	{
		printf("====================================\n||Error: Wrong number of arguments||\n====================================\n");
		return 1;
	}
	i = 1;
	while(i < ac)
	{
		if (check_num(av[i]) == 1)
		{
			printf("===========================\n||Error: invalid argument||\n===========================\n");
			return 1;
		}
		i++;
	}
	begin = (t_begin *)malloc(sizeof(t_begin));
	begin->times = create_times(ft_atoi(av[2]), ft_atoi(av[3]), ft_atoi(av[4]));
	begin->forks_list = NULL;
	i = 0;
	while (i < ft_atoi(av[1]))
	{
		add_node(&begin->forks_list, create_node(1, i + 1));
		i++;
	}
	if (ac == 6)
	{
		begin->times_ate = (int *)malloc(sizeof(int) * ft_atoi(av[1]));
		begin->times->times_must_eat = ft_atoi(av[5]);
	}
	else
	{
		begin->times_ate = (int *)malloc(sizeof(int) * ft_atoi(av[1]));
		begin->times->times_must_eat = -1;
	}
    create_threads(begin, ft_atoi(av[1]));
	free(begin->times);
	free_list(&begin->forks_list);
	free(begin->times_ate);
	free(begin);
	return 0;
}
