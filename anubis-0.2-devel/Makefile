# $Id, blah
CC=gcc
FLAGS=-Isrc/include -o
EXTRA=-c
all: server
server:
	$(CC) $(EXTRA) $(FLAGS) src/server/connection.o src/server/connection.c
	$(CC) $(EXTRA) $(FLAGS) src/server/init_session.o src/server/init_session.c
	$(CC) $(EXTRA) $(FLAGS) src/server/authorize.o src/server/authorize.c
	$(CC) $(EXTRA) $(FLAGS) src/server/md5parse.o src/server/md5parse.c
	$(CC) $(EXTRA) $(FLAGS) src/server/checkkey.o src/server/checkkey.c
	$(CC) $(FLAGS) src/tools/add_node src/tools/add_node.c src/server/resolve.c
	$(CC) $(FLAGS) bin/server src/server/main.c src/server/connection.o src/server/init_session.o src/server/authorize.o src/server/read_config.c src/server/llists.c src/server/md5/md5c.o src/server/md5/mddriver.c src/server/md5parse.o src/server/checkkey.o
	rm -rf *.o *~
clean:
	rm -f src/server/*.o bin/server src/tools/add_node
