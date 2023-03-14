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

void	philosopher(t_forks *utils, int index, int num_philo, int &start)
{
    //como e que passo o start para a funcao?
    while ((int)start < num_philo)
	while (1)
	{
		if (utils->left_fork->value == 1) {
			utils->left_fork->value = 0;
			if (utils->right_fork->value == 1) {
				utils->right_fork->value = 0;
				// eat
				usleep(5000);
			}
			else
				utils->left_fork->value = 1;
        }
	}
}

void    create_threads(t_node *forks, int num_philo)
{
    int i;
    int *j;
    t_forks *utils;
    t_thread *threads;

    i = 0;
    j = malloc(sizeof(int));
    j = 0;
    threads = (t_thread *)malloc(sizeof(t_thread) * num_philo);
    while (i < num_philo)
    {
        utils = assign_forks(forks, i + 1);
        //how do these work?
        pthread_create(&threads[i].thread, NULL, philosopher(utils, i + 1, num_philo, &j), NULL);
        j++;
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
