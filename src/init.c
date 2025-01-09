/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astoll <astoll@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 08:46:21 by astoll            #+#    #+#             */
/*   Updated: 2024/12/19 08:46:21 by astoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	start_threads(t_table *table)
{
	int	i;
	int	j;
	int	groups;

	if (table->nb_philos % 2)
		groups = 3;
	else
		groups = 2;
	i = 0;
	while (i < groups)
	{
		j = i;
		while (j < table->nb_philos)
		{
			pthread_create(&table->philo[j].thread, NULL, routine,
				&table->philo[j]);
			j += groups;
		}
		i++;
		usleep(100);
	}
}

void	init_philos(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->nb_philos)
	{
		table->philo[i].id = i;
		table->philo[i].meals = 0;
		table->philo[i].last_meal = get_current_time();
		table->philo[i].available_fork = true;
		pthread_mutex_init(&table->philo[i].fork_lock, NULL);
		pthread_mutex_init(&table->philo[i].state_lock, NULL);
		table->philo[i].table = table;
		table->philo[i].state = THINKING;
		table->philo[i].s_philo = &table->philo[(i + 1) % table->nb_philos];
	}
	start_threads(table);
	i = -1;
	while (++i < table->nb_philos)
		pthread_join(table->philo[i].thread, NULL);
}

void	init_table(int argc, char **argv)
{
	t_table	table;

	table.philo = ft_calloc(ft_atoi(argv[1]), sizeof(t_philo));
	if (!table.philo)
		return ;
	table.start_time = get_current_time();
	table.nb_philos = ft_atoi(argv[1]);
	table.time_to_die = ft_atoi(argv[2]);
	table.time_to_eat = ft_atoi(argv[3]);
	table.time_to_sleep = ft_atoi(argv[4]);
	table.nb_meals = -1;
	pthread_mutex_init(&table.print_lock, NULL);
	if (argc == 6)
		table.nb_meals = ft_atoi(argv[5]);
	init_philos(&table);
	free(table.philo);
}
