SRCDIR = Source/
CC = gcc
CFLAGS = -Wall -Wextra -Wformat -std=gnu11
CFLAGS_PARALLEL = $(CFLAGS) -fopenmp

.PHONY: all clean origin origin_time parallel parallel_time

all: origin parallel

parallel_time: parallel
	./time_parallel

origin_time: origin
	./time_origin

origin: $(SRCDIR)origin_code.c
	$(CC) $(SRCDIR)origin_code.c -o origin $(CFLAGS) -fopenmp

parallel: $(SRCDIR)paralleled_code.c
	$(CC) $(SRCDIR)paralleled_code.c -o parallel $(CFLAGS_PARALLEL)

clean:
	rm -rf *.o origin parallel
