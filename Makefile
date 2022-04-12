NAME = fractol
MLX = libmlx.a
INCL = /usr/include/../lib
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
CC = clang
FLAGS = -Wall -Wextra -Werror -pthread -Ofast -Lmini -lX11 -lmlx -Imini -lXext

%.o : %.c fratol.h
	$(CC) -Wall -Wextra -Werror -pthread -Ofast -Lmini -lX11 -lmlx -Imini -lXext -c $<


$(NAME) : $(OBJ) $(MLX)
	$(CC) $< $(FLAGS) -o $@

$(MLX):
	make -C mini

clean : 
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean $(NAME)
