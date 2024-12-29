/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 19:35:59 by lpolizzi          #+#    #+#             */
/*   Updated: 2024/12/26 20:42:27 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	res;

	i = 0;
	sign = 1;
	res = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	while (str[i] >= '0' && str[i] <= '9')
		res = res * 10 + str[i++] - '0';
	return (res * sign);
}

size_t	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

size_t	time_diff(size_t past, size_t present)
{
	return (present - past);
}

void	ft_usleep(size_t time)
{
	size_t	start;

	start = get_time();
	while (time_diff(start, get_time()) < time)
		usleep(500);
}

void	print_action(t_data *data, int id, char *action)
{
	pthread_mutex_lock(&(data->printing_lock));
	if (!data->dead_philo)
		printf("%zu %d %s\n", time_diff(data->start_time, get_time()), id + 1,
			action);
	pthread_mutex_unlock(&(data->printing_lock));
}
