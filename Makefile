CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -O2 -I./include
LDFLAGS = -lSDL2 -fsanitize=address,undefined

SRC = src/main.c src/chip8.c
OBJ = $(SRC:.c=.o)
EXEC = chip8-vm


all: $(EXEC)
$(EXEC) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean