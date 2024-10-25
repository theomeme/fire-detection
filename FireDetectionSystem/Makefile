# Makefile
CC = gcc
CFLAGS = -pthread -Wall
OBJ = main.o functions.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

fire_detection_system: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o fire_detection_system