/*
 * test-config-file.c
 *
 * This is a test binary to check the configuration file has been 
 * created in the correct way ...
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
#include <netinet/in.h>
#include "structs.h"

int read_the_config_file(char *cfilename) {
	FILE *inFile;
	struct _filenode *filenode;
	unsigned long tempsize;

	if((inFile = fopen(cfilename, "r")) == NULL) {
		perror("fopen");
		return -1;
	}

	while(1) {
		filenode = (struct _filenode *)malloc(sizeof(struct _filenode));
		if((fread(&filenode->serverkey, sizeof(uint32_t), 1, inFile)) == 0) {
//			printf("cannot read server key\n");
			break;
		}
#ifdef DEBUG
		printf("%u\n", filenode->serverkey);
#endif
		if((fread(&tempsize, sizeof(unsigned long), 1, inFile)) == 0) {
			printf("Error reading\n");
			return -1;
		}
		filenode->sfname = (char *)malloc(tempsize + 1);
		if((fread(filenode->sfname, tempsize, 1, inFile)) == 0)
			return -1;

		filenode->sfname[tempsize] = '\0';
#ifdef DEBUG
		printf("%s\n", filenode->sfname);
#endif
		if((fread(&tempsize, sizeof(unsigned long), 1, inFile)) == 0)
			return -1;
		filenode->lfname = (char *)malloc(tempsize + 1);
		if((fread(filenode->lfname, tempsize, 1, inFile)) == 0)
			return -1;
		
		filenode->lfname[tempsize] = '\0';
#ifdef DEBUG
		printf("%s\n", filenode->lfname);
#endif
		filenode->fingerprint = (char *)malloc(16);
		if((fread(filenode->fingerprint, 16, 1, inFile)) == 0)
			return -1;
#ifdef DEBUG
		MDPrint(filenode->fingerprint);
#endif
				
		add_filenode(filenode);
	}

	fclose(inFile);

	return 0;
}
