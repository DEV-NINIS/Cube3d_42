/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_events.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmassin <gmassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:52:43 by gmassin           #+#    #+#             */
/*   Updated: 2025/12/12 10:28:25 by gmassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

int	on_key_press(int keycode, t_engine *engine)
{
	if (keycode == KEY_ESC)
	{
		engine_destroy(engine);
		exit(0);
	}
	if (keycode == KEY_W)
		engine->keys.move_forward = 1;
	else if (keycode == KEY_S)
		engine->keys.move_backward = 1;
	else if (keycode == KEY_A)
		engine->keys.strafe_left = 1;
	else if (keycode == KEY_D)
		engine->keys.strafe_right = 1;
	else if (keycode == KEY_LEFT)
		engine->keys.turn_left = 1;
	else if (keycode == KEY_RIGHT)
		engine->keys.turn_right = 1;
	return (0);
}

int	on_key_release(int keycode, t_engine *engine)
{
	if (keycode == KEY_W)
		engine->keys.move_forward = 0;
	else if (keycode == KEY_S)
		engine->keys.move_backward = 0;
	else if (keycode == KEY_A)
		engine->keys.strafe_left = 0;
	else if (keycode == KEY_D)
		engine->keys.strafe_right = 0;
	else if (keycode == KEY_LEFT)
		engine->keys.turn_left = 0;
	else if (keycode == KEY_RIGHT)
		engine->keys.turn_right = 0;
	return (0);
}
