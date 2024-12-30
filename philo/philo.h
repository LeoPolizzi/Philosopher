/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 18:59:28 by lpolizzi          #+#    #+#             */
/*   Updated: 2024/12/30 14:48:29 by lpolizzi         ###   ########.fr       */
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

/* error_handling.c */
void				exit_simulation(t_data *data, char *error);
bool				all_digit(char *str);
int					args_invalid(int ac, char **av);

/* utils.c */
int					ft_atoi(const char *str);
size_t				get_time(void);
void				ft_usleep(size_t time);
void				print_action(t_data *data, int id, char *action);

/* init.c */
void				init_data(t_data *data, char **av);
void				init_forks(t_data *data);
void				init_philo(t_data *data);

/* main.c */
void				create_threads(t_data *data);

/* monitoring.c */
int					check_time_diff(t_philo *philo, size_t time_to_die);
bool				check_death(t_data *data);
bool				check_all_ate(t_data *data);
void				*monitoring(void *arg);

/* routine_actions.c */
bool				is_philo_dead(t_data *data);
void				think(t_philo *philo);
void				dream(t_philo *philo);
void				eat(t_philo *philo);
void				*philo_routine(void *arg);

#endif
