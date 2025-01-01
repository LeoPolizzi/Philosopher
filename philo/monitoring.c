/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:07:30 by lpolizzi          #+#    #+#             */
/*   Updated: 2024/12/30 14:08:42 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_time_diff(t_philo *philo, size_t time_to_die)
{
	pthread_mutex_lock(&philo->data->eat_lock);
	if (get_time() - philo->last_meal >= time_to_die && !philo->is_eating)
	{
		pthread_mutex_unlock(&philo->data->eat_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->eat_lock);
	return (0);
}

bool	check_death(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_philo)
	{
		if (check_time_diff(&data->philo[i], data->time_to_die))
		{
			pthread_mutex_lock(&data->death_lock);
			print_action(data, i + 1, "died");
			data->dead_philo = true;
			pthread_mutex_unlock(&data->death_lock);
			return (true);
		}
	}
	return (false);
}

bool	check_all_ate(t_data *data)
{
	int	i;
	int	finished;

	i = -1;
	finished = 0;
	if (data->nb_eat == -1)
		return (false);
	while (++i < data->nb_philo)
	{
		pthread_mutex_lock(&data->eat_lock);
		if (data->philo[i].meal_count >= data->nb_eat)
			finished++;
		pthread_mutex_unlock(&data->eat_lock);
	}
	if (finished == data->nb_philo)
	{
		pthread_mutex_lock(&data->death_lock);
		data->dead_philo = true;
		pthread_mutex_unlock(&data->death_lock);
		return (true);
	}
	return (false);
}

void	*monitoring(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
		if (check_death(data) || check_all_ate(data))
			break ;
	return (NULL);
}
