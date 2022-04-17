NAME = fractol
MLX = libmlx.a
INCL = vendor/mini
LIB = vendor/mini
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
CC = clang
CFLAGS = -mavx2 -mfma -mavx -pthread -Ofast -I$(INCL)
LFLAGS = -L$(LIB) -lmlx -lXext -pthread -lX11

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
