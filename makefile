CC = cc
CFLAGS = -std=c99 -Wall -Wextra -g -I.

SOURCES = headache.c read.c env.c eval.c prnt.c procs.c
 
all: headache run

headache:	
	$(CC) $(SOURCES) $(CFLAGS) -o headache.exe
	
run:
	headache.exe test.ache

clean:
	del headache.exe
