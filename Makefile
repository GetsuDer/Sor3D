SRCDIR = Source/
CC = gcc
MPI_CC = mpicc
CFLAGS = -Wall -Wextra -Wformat -std=gnu11 -g
CFLAGS_OPENMP = -fopenmp

.PHONY: all clean origin origin_time mpi openmp parallel_time

all: origin mpi openmp

data: all_out make_good.py
	cat out_* > all_out; python3 make_good.py

parallel_time: parallel
	./time_parallel

origin_time: origin
	./time_origin

origin: $(SRCDIR)origin_code.c
	$(CC) $(SRCDIR)origin_code.c -o origin $(CFLAGS) $(CFLAGS_OPENMP)

mpi: $(SRCDIR)mpi_code.c
	$(MPI_CC) $(SRCDIR)mpi_code.c -o mpi $(CFLAGS) $(CFLAGS_MPI)

openmp: $(SRCDIR)openmp_code.c
	$(CC) $(SRCDIR)openmp_code.c -o openmp $(CFLAGS) $(CFLAGS_OPENMP)

clean:
	rm -rf *.o origin openmp mpi
