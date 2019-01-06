/*
 * add_node.c
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
#include <stdlib.h>
#include <netinet/in.h>
#include "defs.h"
int main(int argc, char **argv) {
	unsigned char buf[MAX_BUF_SIZE/2];
	uint32_t nbo, anbo;
	FILE *fp;

	if(argc != 2) {
		fprintf(stderr,"%s <server>\n",argv[0]);
		return;
	}
	
	fp = fopen(SERVER_KEY_LIST,"raw+");
	if(!fp) {
		printf("Can't open %s\n",SERVER_KEY_LIST);
		return;
	}
	
	nbo = resolve(argv[1]);	
	if(!nbo) {
		printf("Can't resolve %s\n",argv[1]);
		return;
	}

	memset(buf,0,sizeof(buf));
	fseek(fp,0,SEEK_SET);
	for(;;) {
		if(feof(fp)) break;
		anbo=0;
		memset(buf,0,strlen(buf));
		
		fgets(buf,sizeof(buf)-1,fp);
		if(strlen(buf) <= 2) continue;
		else if(buf[0] == '/' &&
				buf[1] == '/') continue;
		
		if(isspace(buf[strlen(buf)-1])) buf[strlen(buf)-1]=0;
		
		anbo=atol(buf);
		if(anbo==nbo) {
			printf("%s already exist!\n",argv[1]);
			return;
		}
	}
	
	printf("Adding %s... ",argv[1]);
	fflush(stdout);
	fseek(fp,0,SEEK_END);
	fprintf(fp,"%u\n",nbo);
	printf("done.\n");
	
	fclose(fp);
	return;
}
