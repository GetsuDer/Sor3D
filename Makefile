SRCDIR = Source/
CC = g++
CFLAGS = -Wall -Wextra -Wformat -std=gnu++17
CFLAGS_PARALLEL = $(CFLAGS) -fopenmp

.PHONY: all clean origin origin_time parallel parallel_time

all: origin parallel

parallel_time: parallel
	./time_parallel

origin_time: origin
	./time_origin

origin: $(SRCDIR)origin_code.cpp
	$(CC) $(SRCDIR)origin_code.cpp -o origin $(CFLAGS)

parallel: $(SRCDIR)paralleled_code.cpp
	$(CC) $(SRCDIR)paralleled_code.cpp -o parallel $(CFLAGS_PARALLEL)

clean:
	rm -rf *.o origin parallel
