/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astoll <astoll@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:33:56 by astoll            #+#    #+#             */
/*   Updated: 2024/10/01 15:26:38 by astoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

typedef struct s_table
{
	time_t			start_time;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	unsigned int	must_eat_count;
	unsigned int	nb_philos;
	bool			sim_status;
	pthread_mutex_t	sim_status_lock;
	pthread_mutex_t	*fork_locks;
	pthread_mutex_t	write_lock;
	t_philo			**philos;
}	t_table;

typedef struct s_philo
{
	pthread			thread;
	unsigned int	id;
	unsigned int	meals_eaten;
	unsigned int	fork[2];
	time_t			last_meal;
	pthread_mutex_t	meal_time_lock;
	t_table			*table;
}	t_philo;








#endif