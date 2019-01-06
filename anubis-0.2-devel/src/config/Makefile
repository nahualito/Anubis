CC=gcc
CFLAGS=-O3

all:	cwrite test-config-file

cwrite:
	gcc -o cwrite cwrite.c md5/md5c.c md5/mddriver.c resolve.c

test-config-file:
	gcc -o test-config-file test-config-file.c resolve.c llists.c md5/mddriver.c md5/md5c.c

clean:
	rm -rf cwrite test-config-file *~
