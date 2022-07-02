/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   task.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 23:10:33 by yakhoudr          #+#    #+#             */
/*   Updated: 2022/07/02 23:28:58 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void exec_task(t_task *task)
{
	task->draw(task->info, task->sx, task->sy, task->fx, task->fy, task->max_iteration);
	printf("quarter drawn\n");
}

void submit_task(t_task task)
{
	pthread_mutex_lock(&q_mutex);
	Tq[task_count] = task;
	task_count += 1;
	pthread_mutex_unlock(&q_mutex);
	pthread_cond_signal(&wait_cond);
}

void	*start_thread(void *args)
{
	(void) args;
	while (1)
	{
		t_task task;
		pthread_mutex_lock(&q_mutex);
		while (task_count == 0)
			pthread_cond_wait(&wait_cond, &q_mutex);
		task = Tq[0];
		int	i;
		for (i = 0;i < task_count - 1;i += 1)
		{
				Tq[i] = Tq[i + 1];
		}
		task_count -= 1;
		pthread_mutex_unlock(&q_mutex);
		exec_task(&task);
	}
}
