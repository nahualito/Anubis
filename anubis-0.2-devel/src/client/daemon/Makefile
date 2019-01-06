all:	client

client:	Anubis-c.c network.c md5parse.c resolve.c read_config.c llists.c
	gcc -o Anubis-c Anubis-c.c network.c resolve.c read_config.c llists.c md5parse.c -I../../include/

debug:	Anubis-c.c network.c md5parse.c resolve.c read_config.c llists.c
	gcc -o Anubis-c Anubis-c.c network.c resolve.c read_config.c llists.c md5parse.c -I../../include/ -DDEBUG