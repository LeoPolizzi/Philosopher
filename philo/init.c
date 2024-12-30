/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:03:59 by lpolizzi          #+#    #+#             */
/*   Updated: 2024/12/30 14:04:11 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_data(t_data *data, char **av)
{
	data->nb_philo = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
		data->nb_eat = ft_atoi(av[5]);
	else
		data->nb_eat = -1;
	data->dead_philo = false;
	data->start_time = get_time();
	pthread_mutex_init(&data->printing_lock, NULL);
	pthread_mutex_init(&data->death_lock, NULL);
	pthread_mutex_init(&data->eat_lock, NULL);
}

void	init_forks(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_philo)
		pthread_mutex_init(&data->forks[i], NULL);
}

void	init_philo(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_philo)
	{
		data->philo[i].id = i + 1;
		data->philo[i].is_eating = false;
		data->philo[i].meal_count = 0;
		data->philo[i].last_meal = get_time();
		data->philo[i].left_fork = &data->forks[i];
		if (i == 0)
			data->philo[i].right_fork = &data->forks[data->nb_philo - 1];
		else
			data->philo[i].right_fork = &data->forks[i - 1];
		data->philo[i].data = data;
	}
}
