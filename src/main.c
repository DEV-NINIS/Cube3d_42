#include "cube3d.h"
#include <stdlib.h>

static int	on_window_close(t_engine *engine)
{
	engine_destroy(engine);
	exit(0);
	return (0);
}

int	main(int argc, char **argv)
{
	t_engine	engine;

	if (!engine_init(&engine, argc, argv))
		return (1);
	mlx_loop_hook(engine.mlx, &render_frame, &engine);
	mlx_hook(engine.win, 17, 0, &on_window_close, &engine);
	mlx_hook(engine.win, 2, 1L << 0, &on_key_press, &engine);
	mlx_hook(engine.win, 3, 1L << 1, &on_key_release, &engine);
	mlx_loop(engine.mlx);

	return (on_window_close(&engine), 0);
}
