/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astoll <astoll@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 08:46:29 by astoll            #+#    #+#             */
/*   Updated: 2024/12/19 08:46:29 by astoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eating(t_philo *philo)
{
	if (is_someone_dead(philo->table))
		return ;
	if (get_current_time() - philo->last_meal >= philo->table->time_to_eat)
	{
		philo->meals++;
		pthread_mutex_lock(&philo->fork_lock);
		philo->available_fork = true;
		pthread_mutex_unlock(&philo->fork_lock);
		pthread_mutex_lock(&philo->s_philo->fork_lock);
		philo->s_philo->available_fork = true;
		pthread_mutex_unlock(&philo->s_philo->fork_lock);
		if (enough_meals(philo))
			return ;
		change_state(philo, EATING);
		pthread_mutex_lock(&philo->table->print_lock);
		printf("%lu %d is sleeping\n",
			get_current_time() - philo->table->start_time, philo->id + 1);
		pthread_mutex_unlock(&philo->table->print_lock);
	}
}

void	sleeping(t_philo *philo)
{
	if (is_someone_dead(philo->table))
		return ;
	if (get_current_time() - philo->last_meal >= philo->table->time_to_eat
		+ philo->table->time_to_sleep)
		change_state(philo, SLEEPING);
	if (philo->table->nb_philos % 2)
		usleep(philo->table->time_to_sleep);
}

void	thinking(t_philo *philo)
{
	if (is_someone_dead(philo->table))
		return ;
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d is thinking\n", get_current_time()
		- philo->table->start_time, philo->id + 1);
	pthread_mutex_unlock(&philo->table->print_lock);
	change_state(philo, THINKING);
}

void	die(t_philo *philo)
{
	if (philo->table->time_to_die < get_current_time() - philo->last_meal)
	{
		if (!is_someone_dead(philo->table))
		{
			change_state(philo, DEAD);
			pthread_mutex_lock(&philo->table->print_lock);
			printf("%lu %d died\n",
				get_current_time() - philo->table->start_time,
				philo->id + 1);
			pthread_mutex_unlock(&philo->table->print_lock);
		}
	}
}

void	*routine(void *param)
{
	t_philo	*philo;

	philo = (t_philo *)param;
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d is thinking\n", get_current_time()
		- philo->table->start_time, philo->id + 1);
	pthread_mutex_unlock(&philo->table->print_lock);
	while (!is_someone_dead(philo->table) && !enough_meals(philo))
	{
		if (check_state(philo, THINKING))
			take_both_forks(philo);
		if (check_state(philo, TOOK_FORKS))
			eating(philo);
		if (check_state(philo, EATING))
			sleeping(philo);
		if (check_state(philo, SLEEPING))
			thinking(philo);
		die(philo);
		usleep(5);
	}
	return (NULL);
}
