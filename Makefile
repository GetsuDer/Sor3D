TESTDIR = Testing/
SRCDIR = Source/
OBJDIR = ObjectFiles/
INCDIR = Include/
CC = gcc
DEBUG = YES
CFLAGS = -Wall -Wextra -Wformat -std=c17 -IInclude -ISource 

.PHONY: all clean origin origin_time

origin_time: origin
	./time_origin

origin: $(SRCDIR)origin_code.c
	$(CC) $(SRCDIR)origin_code.c -o origin $(CFLAGS)

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -rf *.o ObjectFiles origin
