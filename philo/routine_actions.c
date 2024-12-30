/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_actions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:05:25 by lpolizzi          #+#    #+#             */
/*   Updated: 2024/12/30 14:08:27 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_philo_dead(t_data *data)
{
	pthread_mutex_lock(&data->death_lock);
	if (data->dead_philo)
	{
		pthread_mutex_unlock(&data->death_lock);
		return (true);
	}
	pthread_mutex_unlock(&data->death_lock);
	return (false);
}

void	think(t_philo *philo)
{
	print_action(philo->data, philo->id, "is thinking");
}

void	dream(t_philo *philo)
{
	print_action(philo->data, philo->id, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	print_action(philo->data, philo->id, "has taken a fork");
	if (philo->data->nb_philo == 1)
	{
		ft_usleep(philo->data->time_to_die);
		pthread_mutex_unlock(philo->right_fork);
		return ;
	}
	pthread_mutex_lock(philo->left_fork);
	print_action(philo->data, philo->id, "has taken a fork");
	philo->is_eating = true;
	print_action(philo->data, philo->id, "is eating");
	pthread_mutex_lock(&philo->data->eat_lock);
	philo->last_meal = get_time();
	philo->meal_count++;
	pthread_mutex_unlock(&philo->data->eat_lock);
	ft_usleep(philo->data->time_to_eat);
	philo->is_eating = false;
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2)
		ft_usleep(10);
	while (!is_philo_dead(philo->data))
	{
		eat(philo);
		dream(philo);
		think(philo);
	}
	return (NULL);
}
