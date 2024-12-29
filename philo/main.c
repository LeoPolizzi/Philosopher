/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 19:23:45 by lpolizzi          #+#    #+#             */
/*   Updated: 2024/12/26 20:42:25 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	exit_simulation(t_data *data, char *error)
{
	int	i;

	i = 0;
	if (error)
		write(2, error, ft_strlen(error));
	while (i < data->nb_philo)
		pthread_mutex_destroy(&data->forks[i++]);
	pthread_mutex_destroy(&data->printing_lock);
	pthread_mutex_destroy(&data->death_lock);
	pthread_mutex_destroy(&data->eat_lock);
	exit(EXIT_FAILURE);
}

static bool	all_digit(char *str)
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

static int	args_invalid(int ac, char **av)
{
	if (ac < 5 || ac > 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (false);
	}
	if ((ft_atoi(av[1]) < 1 || atoi(av[1]) > MAX_PHILO) && !all_digit(av[1]))
		return (write(2, "Error: Invalid number of philosophers\n", 36));
	if (ft_atoi(av[2]) <= 0 && !all_digit(av[2]))
		return (write(2, "Error: Invalid time to die\n", 25));
	if (ft_atoi(av[3]) <= 0 && !all_digit(av[3]))
		return (write(2, "Error: Invalid time to eat\n", 26));
	if (ft_atoi(av[4]) <= 0 && !all_digit(av[4]))
		return (write(2, "Error: Invalid time to sleep\n", 28));
	if (ac == 6 && (ft_atoi(av[5]) <= 0 && !all_digit(av[5])))
		return (write(2, "Error: Invalid number of times to eat\n", 38));
	return (0);
}

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

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
}

void	init_philo(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		data->philo[i].id = i;
		data->philo[i].is_eating = false;
		data->philo[i].meal_count = 0;
		data->philo[i].last_meal = get_time();
		data->philo[i].left_fork = &data->forks[i];
		if (i == 0)
			data->philo[i].right_fork = &data->forks[data->nb_philo - 1];
		else
			data->philo[i].right_fork = &data->forks[i - 1];
		data->philo[i].data = data;
		i++;
	}
}

bool	is_dead(t_data *data)
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
	while (!is_dead(philo->data))
	{
		eat(philo);
		dream(philo);
		think(philo);
	}
	return (NULL);
}

int	check_time_diff(t_philo *philo, size_t time_to_die)
{
	pthread_mutex_lock(&philo->data->eat_lock);
	if (get_time() - philo->last_meal >= time_to_die
		&& !philo->is_eating)
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

	i = 0;
	while (i < data->nb_philo)
	{
		if (check_time_diff(&data->philo[i], data->time_to_die))
		{
			print_action(data, i, "died");
			pthread_mutex_lock(&data->death_lock);
			data->dead_philo = true;
			pthread_mutex_unlock(&data->death_lock);
			return (true);
		}
		i++;
	}
	return (false);
}

bool	check_all_ate(t_data *data)
{
	int	i;
	int	finished;

	if (data->nb_eat == -1)
		return (false);
	i = -1;
	finished = 0;
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
		if (!check_death(data) && !check_all_ate(data))
			break ;
	return (NULL);
}

void	create_threads(t_data *data)
{
	pthread_t	observer_thread;
	int			i;

	i = -1;
	if (pthread_create(&observer_thread, NULL, &monitoring, data))
		exit_simulation(data, "Error: Failed to create observer thread\n");
	while (++i < data->nb_philo)
		if (pthread_create(&data->philo[i].thread_id, NULL, &philo_routine,
				&data->philo[i]))
			exit_simulation(data,
				"Error: Failed to create philosopher thread\n");
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
