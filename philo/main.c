/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 19:23:45 by lpolizzi          #+#    #+#             */
/*   Updated: 2024/12/30 14:08:40 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	create_threads(t_data *data)
{
	pthread_t	observer_thread;
	int			i;

	data->start_time = get_time();
	i = -1;
	while (++i < data->nb_philo)
	{
		data->philo[i].last_meal = get_time();
		if (pthread_create(&data->philo[i].thread_id, NULL, &philo_routine,
				&data->philo[i]))
			exit_simulation(data,
				"Error: Failed to create philosopher thread\n");
	}
	if (pthread_create(&observer_thread, NULL, &monitoring, data))
		exit_simulation(data, "Error: Failed to create observer thread\n");
	if (pthread_join(observer_thread, NULL))
		exit_simulation(data, "Error: Failed to join observer thread\n");
	i = -1;
	while (++i < data->nb_philo)
		if (pthread_join(data->philo[i].thread_id, NULL))
			exit_simulation(data, "Error: Failed to join philosopher thread\n");
}

int	main(int ac, char **av)
{
	t_data	data;

	if (args_invalid(ac, av))
		return (EXIT_FAILURE);
	init_data(&data, av);
	init_forks(&data);
	init_philo(&data);
	create_threads(&data);
	exit_simulation(&data, NULL);
	return (0);
}
