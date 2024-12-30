/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:03:06 by lpolizzi          #+#    #+#             */
/*   Updated: 2024/12/30 14:03:36 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	exit_simulation(t_data *data, char *error)
{
	int	i;

	i = 0;
	if (error)
		printf("%s", error);
	while (i < data->nb_philo)
		pthread_mutex_destroy(&data->forks[i++]);
	pthread_mutex_destroy(&data->printing_lock);
	pthread_mutex_destroy(&data->death_lock);
	pthread_mutex_destroy(&data->eat_lock);
	exit(EXIT_FAILURE);
}

bool	all_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

int	args_invalid(int ac, char **av)
{
	if (ac < 5 || ac > 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (1);
	}
	if (ac >= 2 && (ft_atoi(av[1]) < 1 || ft_atoi(av[1]) > MAX_PHILO)
		&& !all_digit(av[1]))
		return (printf("Error: Invalid number of philosophers\n"));
	if (ac >= 3 && ft_atoi(av[2]) <= 0 && !all_digit(av[2]))
		return (printf("Error: Invalid time to die\n"));
	if (ac >= 4 && ft_atoi(av[3]) <= 0 && !all_digit(av[3]))
		return (printf("Error: Invalid time to eat\n"));
	if (ac >= 5 && ft_atoi(av[4]) <= 0 && !all_digit(av[4]))
		return (printf("Error: Invalid time to sleep\n"));
	if (ac == 6 && (ft_atoi(av[5]) <= 0 && !all_digit(av[5])))
		return (printf("Error: Invalid number of times to eat\n"));
	else
		return (0);
}
