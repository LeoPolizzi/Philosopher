/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 18:59:28 by lpolizzi          #+#    #+#             */
/*   Updated: 2024/12/27 23:33:37 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define MAX_PHILO 250

typedef struct s_philo
{
	pthread_t		thread_id;
	int				id;
	bool			is_eating;
	int				meal_count;
	size_t			last_meal;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_eat;
	bool			dead_philo;
	size_t			start_time;
	pthread_mutex_t	printing_lock;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	eat_lock;
	pthread_mutex_t	forks[MAX_PHILO];
	t_philo			philo[MAX_PHILO];
}					t_data;

int		ft_atoi(const char *str);
size_t	get_time(void);
size_t	time_diff(size_t past, size_t present);
void	ft_usleep(size_t time);
void	print_action(t_data *data, int id, char *action);

#endif
