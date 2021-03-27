CC=gcc

CFLAGS= -O3 -std=c11 -Wall -Wextra -lm

OBJECTS:=$(patsubst %.c,%.o,$(wildcard *.c))
EXEC= rooms

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXEC)

clean:
	rm -rf *.o $(EXEC)
