CC = cc
CFLAGS = -std=c99 -Wall -Wextra -g -I.

SOURCES = headache.c read.c env.c eval.c prnt.c
 
all: headache run

headache:	
	$(CC) $(SOURCES) $(CFLAGS) -o headache.exe
	
run:
	headache.exe sample.txt

clean:
	del headache.exe
