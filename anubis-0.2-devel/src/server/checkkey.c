/*
 * checkkey.c
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
#include <sys/socket.h>
#include <netinet/in.h>
#include "defs.h"
int checkkey(uint32_t ias, int fd) {
	FILE *fp;
	uint32_t act;
	char buf[24];
	
	printf("addr %s has server_key %u\n",inet_ntoa(ias),ias);

	fp = fopen(SERVER_KEY_LIST,"r");
	if(!fp) {
		write(fd,MSG_SFILE_ERR,strlen(MSG_SFILE_ERR));
		return 1;
	}

	for(;;) {
		memset(&act,0,sizeof(uint32_t));
		memset(buf,0,sizeof(buf));
		if(feof(fp))
			return 1;
		fgets(buf,sizeof(buf)-1,fp);
		if(isspace(buf[strlen(buf)-1])) buf[strlen(buf)-1]=0;
		if(strlen(buf) < 2) continue;
		if(buf[0] == '/' && buf[1]=='/') continue;
		act=atoll(buf);
		if(act == ias) break;
	}
	fclose(fp);
	return 0;	
}

