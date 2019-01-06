/*
 * cwrite.c
 *
 * This part of Anubis reads th econfig file wich is a 
 * random accessed file.
 *
 * THIS IS PROPIETARY SOURCE CODE OF KELSI SILER AND G-CON
 * THIS ODE BINDA TO THE MAIN LICENSE IN THE LICENSE FILE 
 * THAT IS CONTAINED WITHIN THIS PROGRAM IF YOU RECIEVED THIS
 * FILE WITHOUT THE LICENSE FILE REFER TO http://anubis.g-con.org/LICENSE
 * FOR THE LICENSE
 *
 * Enrique A. Sanchez Montellano
 * nahual@g-con.org
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <netinet/in.h>
#include "structs.h"

int write_to_config_file(char *cfilename, uint32_t serverkey, char *lfname, unsigned char *fingerprint) {
	int fd;
	unsigned long sfname_length;
	unsigned long lfname_length;
	unsigned char *sfname;
	chmod(cfilename, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP | S_IROTH | S_IWOTH);

	if((fd = open(cfilename, O_CREAT | O_WRONLY | O_APPEND)) <= 0) {
		perror("open");
		return -1;
	}

	sfname = strrchr(lfname,'/');
	if(!sfname) sfname=lfname;
	else *sfname++;
	sfname_length = strlen(sfname);
	lfname_length = strlen(lfname);

	printf("writting serverkey: %u\n", serverkey);
	write(fd, &serverkey, sizeof(uint32_t));
	printf("writting sfname: \"%s\"\n", sfname);
	write(fd, &sfname_length, sizeof(unsigned long));
	write(fd, sfname, sfname_length);
	printf("writting lfname: \"%s\" with lenght %d\n", lfname, lfname_length);
	write(fd, &lfname_length, sizeof(unsigned long));
	write(fd, lfname, lfname_length);
	write(fd, fingerprint, 16);
	close(fd);

	return 0;
}

int main(int argc, char **argv) {
	uint32_t address;
	struct stat st;
	
	if(argc < 4) { 
		printf("usage %s <long name> <config file> <ip of this server>\n", argv[0]);
		return -1;
	}
	
	address = resolve(argv[3]);
	if(!address) {
		printf("Can't resolve %s\n",argv[3]);
		return;
	}

	if(stat(argv[1],&st) == -1) {
		printf("%s",argv[1]);
		fflush(stdout);
		perror(" ");
		return;
	}
	
	write_to_config_file(argv[2], address, argv[1], (unsigned char *)MDFile(argv[1]));
	
	return 0;
}
