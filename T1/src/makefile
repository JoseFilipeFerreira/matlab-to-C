CC=gcc

CFLAGS= -std=c11 -Wall -Wextra -lm

OBJECTS:=$(patsubst %.c,%.o,$(wildcard *.c))
EXEC= rooms

$(EXEC): $(OBJECTS)
	$(CC) -O3 $(CFLAGS) $(OBJECTS) -o $(EXEC)

clean:
	rm -rf *.o $(EXEC)
