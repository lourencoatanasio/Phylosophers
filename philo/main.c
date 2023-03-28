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

int is_dead(t_philo *philo, int *died)
{
	if (gt(philo->start_time) - philo->last_eat > philo->times->time_death)
	{
		printf("\033[1;30m%lld ms philo %d died\n\033[0m", gt(((t_philo *)philo)->start_time), ((t_philo *)philo)->index);
		pthread_mutex_unlock(&((t_philo *) philo)->forks->left_fork->mutex);
		if((t_philo *)philo->forks->right_fork != NULL)
			pthread_mutex_unlock(&((t_philo *) philo)->forks->right_fork->mutex);
		*died = 1;
		return (1);
	}
	return (0);
}

int	eat(t_philo *philo, int *died)
{
	((t_philo *)philo)->last_eat = gt(((t_philo *)philo)->start_time);
	if (is_dead(philo, died) == 1 || *died == 1)
		return 1;
	printf("\033[1;91m%lld ms philo %d is eating\n\033[0m", gt(((t_philo *)philo)->start_time), ((t_philo *)philo)->index);
	while(gt(philo->start_time) - philo->last_eat < philo->times->time_eat)
	{
		if (is_dead(philo, died) == 1 || *died == 1)
			return 1;
	}
	return 0;
}

int sleeping(t_philo *philo, int *died)
{
	printf("\033[1;92m%lld ms philo %d is sleeping\n\033[0m", gt(((t_philo *)philo)->start_time), ((t_philo *)philo)->index);
	while(gt(philo->start_time) - philo->last_eat < philo->times->time_sleep + philo->times->time_eat)
	{
		if (is_dead(philo, died) == 1 || *died == 1)
			return 1;
	}
	return 0;
}

void message(t_philo *philo, int *died)
{
	if (is_dead(philo, died) == 1 || *died == 1)
		return ;
	printf("\033[1;93m%lld ms philo %d is thinking\n\033[0m", gt(((t_philo *)philo)->start_time), ((t_philo *)philo)->index);
}

void	*philosopher(void *philo)
{
	static int		died;

	died = 0;
    while (*((t_philo *)philo)->start < ((t_philo *)philo)->num_philo)
		;
	gettimeofday(&((t_philo *)philo)->start_time, NULL);
	if(((t_philo *)philo)->index % 2 == 0)
		usleep(100);
	while(is_dead((t_philo *)philo, &died) == 0) {
		if (died == 1 || is_dead((t_philo *) philo, &died) == 1)
			break;
		pthread_mutex_lock(&((t_philo *) philo)->forks->left_fork->mutex);
		printf("\033[1;34m%lld ms philo %d took left fork\n\033[0m", gt(((t_philo *) philo)->start_time),
			   ((t_philo *) philo)->index);
		if (((t_philo *)philo)->forks->right_fork == NULL)
		{
			while (1) {
				if (is_dead((t_philo *) philo, &died) == 1)
					break;
			}
			break;
		}
		if (died == 1 || is_dead((t_philo *) philo, &died) == 1)
			break ;
		pthread_mutex_lock(&((t_philo *) philo)->forks->right_fork->mutex);
		printf("\033[1;34m%lld ms philo %d took right fork\n\033[0m", gt(((t_philo *) philo)->start_time),
			   ((t_philo *) philo)->index);
		if (eat((t_philo *) philo, &died) == 1)
			break ;
		pthread_mutex_unlock(&((t_philo *) philo)->forks->left_fork->mutex);
		pthread_mutex_unlock(&((t_philo *) philo)->forks->right_fork->mutex);
		if (sleeping((t_philo *) philo, &died) == 1)
			break;
		if (died == 1 || is_dead((t_philo *) philo, &died) == 1)
			break;
		printf("\033[1;93m%lld ms philo %d is thinking\n\033[0m", gt(((t_philo *)philo)->start_time), ((t_philo *)philo)->index);
	}
}

t_philo *create_philo(t_forks *utils, int index, int num_philo, int *start, t_times *times)
{
	t_philo *philo;
	philo = (t_philo *)malloc(sizeof(t_philo));
	philo->times = times;
	philo->forks = utils;
	philo->index = index;
	philo->num_philo = num_philo;
	philo->start = start;
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

void    create_threads(t_node *forks, int num_philo, t_times *times)
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
        utils = assign_forks(forks, i + 1, num_philo);
		philo[i] = create_philo(utils, i + 1, num_philo, &i, times);
        pthread_create(&th[i], NULL, philosopher, philo[i]);
        i++;
    }
    j = 0;
    while (j < num_philo)
    {
        pthread_join(th[j], NULL);
        j++;
    }
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

int main(int ac, char **av)
{
	t_node *forks;
	t_times *times;
	int i;
	if (ac != 6 && ac != 5)
		return 1;
	times = create_times(ft_atoi(av[2]), ft_atoi(av[3]), ft_atoi(av[4]));
	i = 0;
	forks = NULL;
	while (i < ft_atoi(av[1]))
	{
		add_node(&forks, create_node(1, i + 1));
		i++;
	}
	print_list(forks);
    create_threads(forks, ft_atoi(av[1]), times);
//	usleep(5000000);
	free_list(&forks);
	return 0;
}
