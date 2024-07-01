CC = cc
CFLAGS = -Wall -Wextra -g -I.

SOURCES = main.c arraylist.c associativearray.c
 
all: headache run

headache:	
	$(CC) $(SOURCES) $(CFLAGS) -o headache.exe
	
run:
	headache.exe sample.txt

clean:
	del headache.exe
