LFLAGS= -LMLX42/build -lmlx42 -ldl -lglfw -pthread -lm
CFLAGS= -Wall -Wextra -Werror -IMLX42/include
CC=gcc
SRC=$(wildcard src/*.c)
OBJ=$(SRC:src/%.c=obj/%.o)
NAME=bin/fractol


obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $^ $(LFLAGS)  -o $@
clean:
	/usr/bin/rm -rf $(OBJ)
fclean: clean
	/usr/bin/rm -rf $(NAME)

re: fclean all
.PHONY: all clean fclean re
