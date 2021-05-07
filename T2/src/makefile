CC=gcc

CFLAGS= -O3 -lm -fopenmp 
OBJECTS:=$(patsubst %.c,%.o,$(wildcard *.c))
EXEC= poisson

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXEC)

clean:
	rm -rf *.o $(EXEC)
