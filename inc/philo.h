#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_table	t_table;

typedef enum s_state
{
	SLEEPING,
	EATING,
	TOOK_FORKS,
	THINKING,
	DEAD
}	t_state;

typedef struct s_philo
{
	pthread_t			thread;
	int					id;
	int					meals;
	size_t				last_meal;
	bool				available_fork;
	pthread_mutex_t		fork_lock;
	t_state				state;
	pthread_mutex_t		state_lock;
	t_table				*table;
	struct s_philo		*s_philo;

}	t_philo;

typedef struct s_table
{
	t_philo				*philo;
	int					nb_philos;
	int					nb_meals;
	size_t				start_time;
	size_t				time_to_die;
	size_t				time_to_eat;
	size_t				time_to_sleep;
	bool				someone_died;
	pthread_mutex_t		print_lock;
}	t_table;

// main
bool	parsing(int argc, char **argv);

// init
void	start_threads(t_table *table);
void	init_philos(t_table *table);
void	init_table(int argc, char **argv);

// routine
void	eating(t_philo *philo);
void	sleeping(t_philo *philo);
void	thinking(t_philo *philo);
void	die(t_philo *philo);
void	*routine(void *param);

// forks
void	take_both_forks(t_philo *philo);
bool	take_own_fork(t_philo *philo);
bool	take_second_fork(t_philo *philo);

// state
bool	is_someone_dead(t_table *table);
bool	enough_meals(t_philo *philo);
void	change_state(t_philo *philo, t_state state);
bool	check_state(t_philo *philo, t_state state);

// utils
int		ft_atoi(const char *str);
void	*ft_calloc(size_t nmemb, size_t size);
bool	is_digit(char *str);
size_t	get_current_time(void);

#endif