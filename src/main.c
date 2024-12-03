#include "../inc/philo.h"
//main

bool	parsing(int argc, char **argv)
{
	int	i;
	int	value;

	i = 1;
	if (argc < 5 || argc > 6)
		return (false);
	while (i < argc)
	{
		if (!is_digit(argv[i]))
			return (false);
		value = ft_atoi(argv[i]);
		if (i == 1)
		{
			if (value <= 0)
				return (false);
		}
		else
		{
			if (value < 0)
				return (false);
		}
		i++;
	}
	return (true);
}

int	main(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		printf("Invalid number of arguments.\n");
		return (1);
	}
	if (!parsing(argc, argv))
	{
		printf("Invalid arguments.\n");
		return (1);
	}
	init_tables(argc, argv);
	return (0);
}





//init
void	start_threads(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		pthread_create(&table->philo[i].thread, NULL, routine,
			&table->philo[i]);
		i++;
		usleep(100);
	}
}

void	init_philos(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		table->philo[i].id = i;
		table->philo[i].meals = 0;
		table->philo[i].last_meal = get_current_time();
		table->philo[i].fork_available = true;
		pthread_mutex_init(&table->philo[i].lock_fork, NULL);
		pthread_mutex_init(&table->philo[i].lock_state, NULL);
		table->philo[i].table = table;
		table->philo[i].state = THINKING;
		if (table->nb_philos == 1)
			table->philo[i].s_philo = &table->philo[i];
		else
			table->philo[i].s_philo = &table->philo[(i + 1) % table->nb_philos];
		i++;
	}
	start_threads(table);
	i = 0;
	while (i < table->nb_philos)
	{
		pthread_join(table->philo[i].thread, NULL);
		i++;
	}
}

void	init_tables(int argc, char **argv)
{
	t_table	table;
	int		i;

	i = 0;
	table.nb_philos = ft_atoi(argv[1]);
	table.philo = ft_calloc(table.nb_philos, sizeof(t_philo));
	if (!table.philo)
		return ;
	table.start_time = get_current_time();
	table.time_to_death = ft_atoi(argv[2]);
	table.time_to_eat = ft_atoi(argv[3]);
	table.time_to_sleep = ft_atoi(argv[4]);
	table.nb_meals = -1;
	pthread_mutex_init(&table.lock_write, NULL);
	if (argc == 6)
		table.nb_meals = ft_atoi(argv[5]);
	init_philos(&table);
	while (i < table.nb_philos)
	{
		pthread_mutex_destroy(&table.philo[i].lock_fork);
		pthread_mutex_destroy(&table.philo[i].lock_state);
		i++;
	}
	pthread_mutex_destroy(&table.lock_write);
	free(table.philo);
}






//routine
#include "philo.h"

void	think(t_philo *philo)
{
	if (is_someone_dead(philo->table))
		return ;
	pthread_mutex_lock(&philo->table->lock_write);
	printf("%lu %d is thinking\n",
		get_current_time() - philo->table->start_time, philo->id + 1);
	pthread_mutex_unlock(&philo->table->lock_write);
	change_state(philo, THINKING);
}

void	sleepy(t_philo *philo)
{
	if (is_someone_dead(philo->table))
		return ;
	pthread_mutex_lock(&philo->table->lock_write);
	printf("%lu %d is sleeping\n",
		get_current_time() - philo->table->start_time, philo->id + 1);
	pthread_mutex_unlock(&philo->table->lock_write);
	change_state(philo, SLEEPING);
	usleep(philo->table->time_to_sleep * 1000);
}

void	eat(t_philo *philo)
{
	if (is_someone_dead(philo->table))
		return ;
	change_state(philo, EATING);
	pthread_mutex_lock(&philo->table->lock_write);
	printf("%lu %d is eating\n",
		get_current_time() - philo->table->start_time, philo->id + 1);
	pthread_mutex_unlock(&philo->table->lock_write);
	philo->last_meal = get_current_time();
	usleep(philo->table->time_to_eat * 1000);
	philo->meals++;

	// Release forks
	pthread_mutex_lock(&philo->lock_fork);
	philo->fork_available = true;
	pthread_mutex_unlock(&philo->lock_fork);

	pthread_mutex_lock(&philo->s_philo->lock_fork);
	philo->s_philo->fork_available = true;
	pthread_mutex_unlock(&philo->s_philo->lock_fork);
}

bool take_both_forks(t_philo *philo)
{
    if (philo->id % 2 == 0)
    {
        if (take_own_fork(philo))
        {
            if (take_sfork(philo))
                return true;
            // Échec de la prise de s_fork, relâcher own_fork
            pthread_mutex_lock(&philo->lock_fork);
            philo->fork_available = true;
            pthread_mutex_unlock(&philo->lock_fork);
        }
    }
    else
    {
        if (take_sfork(philo))
        {
            if (take_own_fork(philo))
                return true;
            // Échec de la prise de own_fork, relâcher s_fork
            pthread_mutex_lock(&philo->s_philo->lock_fork);
            philo->s_philo->fork_available = true;
            pthread_mutex_unlock(&philo->s_philo->lock_fork);
        }
    }
    usleep(1000); // Petite pause pour éviter de surcharger le processeur
    return false;
}

void	*routine(void *param)
{
	t_philo	*philo;

	philo = (t_philo *)param;
	pthread_mutex_lock(&philo->table->lock_write);
	printf("\033[1;37m%lu \033[1;36m%d is thinking\n\033[0m", get_current_time()
		- philo->table->start_time, philo->id + 1);
	pthread_mutex_unlock(&philo->table->lock_write);
	while (!is_someone_dead(philo->table) && !enough_meals(philo))
	{
		if (check_state(philo, THINKING))
			take_both_forks(philo);
		if (check_state(philo, TOOK_FORKS))
			eat(philo);
		if (check_state(philo, EATING))
			sleepy(philo);
		if (check_state(philo, SLEEPING))
			think(philo);
		die(philo);
		usleep(5);
	}
	return (NULL);
}


//actions
#include "philo.h"

bool	take_own_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->lock_fork);
	if (philo->fork_available)
	{
		philo->fork_available = false;
		pthread_mutex_lock(&philo->table->lock_write);
		printf("%lu %d has taken a fork\n",
			get_current_time() - philo->table->start_time, philo->id + 1);
		pthread_mutex_unlock(&philo->table->lock_write);
		pthread_mutex_unlock(&philo->lock_fork);
		return (true);
	}
	pthread_mutex_unlock(&philo->lock_fork);
	return (false);
}

bool	take_sfork(t_philo *philo)
{
	pthread_mutex_lock(&philo->s_philo->lock_fork);
	if (philo->s_philo->fork_available)
	{
		philo->s_philo->fork_available = false;
		pthread_mutex_lock(&philo->table->lock_write);
		printf("%lu %d has taken a fork\n",
			get_current_time() - philo->table->start_time, philo->id + 1);
		pthread_mutex_unlock(&philo->table->lock_write);
		pthread_mutex_unlock(&philo->s_philo->lock_fork);
		return (true);
	}
	pthread_mutex_unlock(&philo->s_philo->lock_fork);
	return (false);
}






//simu
#include "philo.h"

void	die(t_philo *philo)
{
	pthread_mutex_lock(&philo->lock_state);
	if (philo->state != DEAD &&
		get_current_time() - philo->last_meal > philo->table->time_to_death)
	{
		philo->state = DEAD;
		pthread_mutex_unlock(&philo->lock_state);
		pthread_mutex_lock(&philo->table->lock_write);
		printf("%lu %d died\n",
			get_current_time() - philo->table->start_time, philo->id + 1);
		pthread_mutex_unlock(&philo->table->lock_write);
	}
	else
		pthread_mutex_unlock(&philo->lock_state);
}

bool	is_someone_dead(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		pthread_mutex_lock(&table->philo[i].lock_state);
		if (table->philo[i].state == DEAD)
		{
			pthread_mutex_unlock(&table->philo[i].lock_state);
			return (true);
		}
		pthread_mutex_unlock(&table->philo[i].lock_state);
		i++;
	}
	return (false);
}

bool	enough_meals(t_philo *philo)
{
	if (philo->table->nb_meals == -1)
		return (false);
	if (philo->meals >= philo->table->nb_meals)
		return (true);
	return (false);
}






//utils
#include "philo.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == 43 || str[i] == 45)
	{
		if (str[i] == 45)
			sign = -sign;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result *= 10;
		result += str[i] - 48;
		i++;
	}
	return (result * sign);
}

bool	is_digit(const char *str)
{
	int	i;

	i = 0;
	if (str == NULL || str[i] == '\0')
		return (false);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

size_t	get_current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	change_state(t_philo *philo, t_state state)
{
	pthread_mutex_lock(&philo->lock_state);
	philo->state = state;
	pthread_mutex_unlock(&philo->lock_state);
}

bool	check_state(t_philo *philo, t_state state)
{
	bool	value;

	pthread_mutex_lock(&philo->lock_state);
	value = philo->state == state;
	pthread_mutex_unlock(&philo->lock_state);
	return (value);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*str;
	size_t			total_size;
	size_t			i;

	total_size = nmemb * size;
	str = malloc(total_size);
	if (!str)
		return (NULL);
	i = 0;
	while (i < total_size)
	{
		str[i] = 0;
		i++;
	}
	return ((void *)str);
}

