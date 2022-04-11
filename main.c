#include <mlx.h>
#include <immintrin.h>

int	main(int ac, char **av)
{
	(void)av;
	if (ac == 2)
	{
		void	*window = mlx_new_window(mlx_init(), 400, 400, "home");
		__m256d _zr;
		(void) window;
	}
}

