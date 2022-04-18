NAME = fractol
MLX = libmlx.a
INCL = /usr/include/../lib
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
CC = clang
CFLAGS = -mavx -mavx2 -mfma -Wall -Wextra -Werror -pthread -Ofast
LFLAGS =  -Lmini -lX11 -lmlx -Imini -lXext

%.o : %.c fratol.h
	$(CC) $(CFLAGS) -c $<


$(NAME) : $(OBJ) $(MLX)
	$(CC) $< $(LFLAGS) -o $@

$(MLX):
	make -C vendor/mini

clean : 
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean $(NAME)
