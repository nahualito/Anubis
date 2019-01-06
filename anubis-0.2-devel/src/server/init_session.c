/*
 * init_session.c
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
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include "defs.h"

int init_session(int fd, uint32_t server_key) {
	int len, size = MAX_BUF_SIZE;
	unsigned char *p, *buf;
	
	buf = (char *)malloc(size);

	buf[size-1]=0;
	
	snprintf(buf,size-1,"%d\t%s", I_BANNER, S_BANNER);
	
	len = write(fd,buf,strlen(buf));
	if(len <=0)
		return -1;

	memset(buf,0,strlen(buf));
	len=authorize(fd,PASSWORD);
	if(len == -1) return;
	
	snprintf(buf,size-1,"%d\t%s", I_ASKMD5, S_ASKMD5);
	len=write(fd,buf,strlen(buf));
	if(len <=0) return;

	len=md5parse(fd,server_key);
	
	return;
}
