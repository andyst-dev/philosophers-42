/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astoll <astoll@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 08:46:34 by astoll            #+#    #+#             */
/*   Updated: 2024/12/19 08:46:34 by astoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_someone_dead(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		if (check_state(&table->philo[i], DEAD))
			return (true);
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

void	change_state(t_philo *philo, t_state state)
{
	pthread_mutex_lock(&philo->state_lock);
	philo->state = state;
	pthread_mutex_unlock(&philo->state_lock);
}

bool	check_state(t_philo *philo, t_state state)
{
	bool	value;

	pthread_mutex_lock(&philo->state_lock);
	value = philo->state == state;
	pthread_mutex_unlock(&philo->state_lock);
	return (value);
}
