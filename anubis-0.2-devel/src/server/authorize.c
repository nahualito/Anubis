/*
 * authorize.c
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
#include "defs.h"

int authorize(int fd, const char *password) {
	int len, size = MAX_BUF_SIZE;
	unsigned char *p, *buf;
	
	buf = (char *)malloc(size);

	memset(buf,0,size);
	
	len=read(fd,buf,size-1);
	if(len<=0) 
		return -1;
	
	if(strlen(buf) < 3) return -1; // Check for NULL passwords
	if(buf[0] != '\t') return -1;
	else *buf++=0;
	
	p = strpbrk(buf,"\r\n");
	if(!p) return -1;
	*p=0; p=buf;
	
	if(strcmp(p,password)) { 
		write(fd,AUTH_FAILED,strlen(AUTH_FAILED));	
		return -1;
	}

	write(fd,AUTH_GRANTED,strlen(AUTH_GRANTED));
	return 0;
}	
