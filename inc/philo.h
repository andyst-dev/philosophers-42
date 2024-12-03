#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
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
}						t_state;

typedef struct s_philo
{
	int					id;
	int					meals;
	t_table				*table;
	pthread_t			thread;
	pthread_mutex_t		lock_state;
	t_state				state;
	pthread_mutex_t		lock_fork;
	bool				fork_available;
	struct s_philo		*s_philo;
	size_t				last_meal;
}						t_philo;

typedef struct s_table
{
	t_philo				*philo;
	int					nb_philos;
	int					nb_meals;
	size_t				time_to_death;
	size_t				time_to_eat;
	size_t				time_to_sleep;
	size_t				start_time;
	bool				someone_died;
	pthread_mutex_t		lock_write;
}						t_table;

int						ft_atoi(const char *str);
void					start_threads(t_table *table);
void					init_philos(t_table *table);
void					init_tables(int argc, char **argv);
bool					enough_meals(t_philo *philo);
bool					is_someone_dead(t_table *table);
void					die(t_philo *philo);
void					*routine(void *param);
bool					check_state(t_philo *philo, t_state state);
void					change_state(t_philo *philo, t_state state);
size_t					get_current_time(void);
void					think(t_philo *philo);
void					sleepy(t_philo *philo);
void					eat(t_philo *philo);
bool					take_both_forks(t_philo *philo);
bool					take_own_fork(t_philo *philo);
bool					take_sfork(t_philo *philo);
bool					is_digit(const char *str);
void					*ft_calloc(size_t nmemb, size_t size);

#endif
