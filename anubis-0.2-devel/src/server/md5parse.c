/*
 * md5parse.c  
 *
 * THIS IS PROPIETARY SOURCE CODE OF KELSI SILER AND G-CON
 * THIS CODE USES THE MAIN LICENSE IN THE LICENSE FILE 
 * THAT IS CONTAINED WITHIN THIS PROGRAM. IF YOU RECIEVED THIS
 * FILE WITHOUT THE LICENSE FILE REFER TO http://anubis.g-con.org/LICENSE
 * FOR THE LICENSE
 *
 * NOTES:
 * 
 * May 26 2003
 * 	- For now we only read into the sfname variable wich is not really
 * 	the short file name just the file name that is sent to the server
 * 	from there we will check what is going on -- Nahual
 *
 * diego@g-con.org
 */

#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include "defs.h"
#include "structs.h"

int md5parse(int fd, uint32_t server_key) {
	struct _cfilenode *fileinfo;
	int len=0;
	char *hlpPtr;
	
	fileinfo->sfname = (unsigned char *)malloc(MAX_BUF_SIZE);

	memset(fileinfo->sfname,0,MAX_BUF_SIZE);

	len=read(fd,fileinfo->sfname,MAX_BUF_SIZE-1);
	if(len<=0) return -1;

	hlpPtr = fileinfo->sfname;

	while(*hlpPtr != ':') {
		if(!hlpPtr) {
			return -1;
		}
		hlpPtr++;
	}

	*hlpPtr++ = '\0';

	if(*hlpPtr == '\0')
		return -1; // in case we get <name>:

	//hlpPtr now points to the key
	if(isspace(hlpPtr[strlen(hlpPtr) - 1]))
		hlpPtr[strlen(hlpPtr) - 1] = '\0';

	fileinfo->serverkey=server_key;

	if(search_node2(fileinfo->sfname, hlpPtr) == 0) {
		write(fd, EXEC_GRANTED, strlen(EXEC_GRANTED));
	}
	else {
		write(fd, EXEC_DENIED, strlen(EXEC_DENIED));
	}

	free(fileinfo->sfname);
	
	return;
}
