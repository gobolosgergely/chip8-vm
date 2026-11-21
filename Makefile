CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -O2 -I./include
LDFLAGS = -lSDL3 -fsanitize=address,undefined

SRC = src/main.c src/chip8.c src/instructions.c src/graphics.c src/audio.c src/config.c
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