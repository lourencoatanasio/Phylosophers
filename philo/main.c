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

t_forks	*assign_forks(t_node *forks, int index)
{
	t_forks	*utils;

	utils = (t_forks *)malloc(sizeof(t_forks));
	utils->left_fork = list_index(forks, index);
	utils->right_fork = list_index(forks, index + 1);
	if (utils->right_fork == NULL)
		utils->right_fork = list_index(forks, 1);
	return (utils);
}

void	eating(t_philo *philo)
{
	philo->forks->right_fork->value = 0;
	usleep(5000);
	printf("Philosopher %d is eating ||\n============================\nLeft_Fork -> %d\nRight_Fork -> %d\n\n", philo->index, philo->forks->left_fork->value, philo->forks->right_fork->value);
	philo->forks->left_fork->value = 1;
	philo->forks->right_fork->value = 1;
}

void	philosopher(t_philo *philo)
{
    while (*philo->start <= philo->num_philo)
		;
	usleep(200);
	gettimeofday(&philo->start_time, NULL);
	printf("Philosopher %d is starting ||\n============================\nLeft_Fork -> %d\nRight_Fork -> %d\n\n", philo->index, philo->forks->left_fork->value, philo->forks->right_fork->value);
	usleep(200);
	gt(philo->start_time);
	while (1)
	{
		if(philo->forks->left_fork->value == 1) {
			philo->forks->left_fork->value = 0;
			if (philo->forks->right_fork->value == 1)
				eating(philo);
		}
	}
}

t_philo *create_philo(t_forks *utils, int index, int num_philo, int *start)
{
	t_philo *philo;
	philo = (t_philo *)malloc(sizeof(t_philo));
	philo->forks = utils;
	philo->index = index;
	philo->num_philo = num_philo;
	philo->start = start;
	return (philo);
}

void    create_threads(t_node *forks, int num_philo)
{
    int i;
    t_forks *utils;
    t_thread *threads;
	t_philo **philo;

    i = 1;
	printf("num_philo	 %d\n", num_philo);
	philo = (t_philo **)malloc(sizeof(t_philo *) * num_philo);
    threads = (t_thread *)malloc(sizeof(t_thread) * num_philo);
    while (i <= num_philo + 1)
    {
        utils = assign_forks(forks, i);
		philo[i] = create_philo(utils, i, num_philo, &i);
		printf("philo->index %d\nphilo->num_philo %d\nphilo->start %d\n\n", philo[i]->index, philo[i]->num_philo, *philo[i]->start);
        //how do these work?
        pthread_create(&threads[i].thread, NULL, (void *)philosopher, philo[i]);
        i++;
    }
    i = 0;
    while (i < num_philo)
    {
        pthread_join(threads[i].thread, NULL);
        i++;
    }
}

int main(int argc, char **argv)
{
	t_node *forks;
	int i;

	if (argc != 6 && argc != 5)
		return 1;
	i = 0;
	forks = NULL;
	while (i < ft_atoi(argv[1]))
	{
		add_node(&forks, create_node(1, i + 1));
		i++;
	}
    create_threads(forks, ft_atoi(argv[1]));
//	usleep(5000000);
	print_list(forks);
	free_list(&forks);
	return 0;
}
