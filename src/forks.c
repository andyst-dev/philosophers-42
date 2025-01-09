/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astoll <astoll@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 08:46:17 by astoll            #+#    #+#             */
/*   Updated: 2024/12/19 08:46:17 by astoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_both_forks(t_philo *philo)
{
	if (is_someone_dead(philo->table))
		return ;
	if (take_own_fork(philo))
	{
		if (take_second_fork(philo))
		{
			pthread_mutex_lock(&philo->table->print_lock);
			printf("%lu %d is eating\n",
				get_current_time() - philo->table->start_time, philo->id + 1);
			pthread_mutex_unlock(&philo->table->print_lock);
			change_state(philo, TOOK_FORKS);
			philo->last_meal = get_current_time();
		}
		else
		{
			pthread_mutex_lock(&philo->fork_lock);
			philo->available_fork = true;
			pthread_mutex_unlock(&philo->fork_lock);
		}
	}
}

bool	take_own_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork_lock);
	if (philo->available_fork)
	{
		philo->available_fork = false;
		pthread_mutex_unlock(&philo->fork_lock);
		return (true);
	}
	pthread_mutex_unlock(&philo->fork_lock);
	return (false);
}

bool	take_second_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->s_philo->fork_lock);
	if (philo->s_philo->available_fork)
	{
		philo->s_philo->available_fork = false;
		pthread_mutex_lock(&philo->table->print_lock);
		printf("%lu %d has taken a fork\n",
			get_current_time() - philo->table->start_time, philo->id + 1);
		pthread_mutex_unlock(&philo->table->print_lock);
		pthread_mutex_unlock(&philo->s_philo->fork_lock);
		return (true);
	}
	pthread_mutex_unlock(&philo->s_philo->fork_lock);
	return (false);
}
