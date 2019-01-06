/*
 * main.c
 *
 * THIS IS PROPIETARY SOURCE CODE OF KELSI SILER AND G-CON
 * THIS CODE USES THE MAIN LICENSE IN THE LICENSE FILE
 * THAT IS CONTAINED WITHIN THIS PROGRAM. IF YOU RECIEVED THIS
 * FILE WITHOUT THE LICENSE FILE REFER TO http://anubis.g-con.org/LICENSE
 * FOR THE LICENSE
 *
 * diego@g-con.org
 */

#include <stdio.h>
#include <unistd.h>
#include "defs.h"

int main(int argc, char **argv) {
	unsigned short port=PORT;
	if(argc == 2) port=atoi(argv[1]);
	if(port <=0 || port > 65535) return;

	printf("Starting anubis server\n");
	printf("Reading the configuration file\t...\t");
	if(read_the_config_file(CONFIG_FILE) == -1) {
		exit(-1);
	}
	printf("DONE!\n");
	connection(port);
	return;
}
