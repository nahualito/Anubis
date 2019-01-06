/*
 * connection.c
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
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <fcntl.h>
int connection(unsigned short port) {
	int fd, client, op;
	struct sockaddr_in cl, sr;

	if((fd = socket(AF_INET,SOCK_STREAM,0)) == -1) {
		perror("socket()");
		return;
	}

	sr.sin_addr.s_addr = INADDR_ANY;
	sr.sin_family = AF_INET;
	sr.sin_port = htons(port);
	
	// Avoid that crappy "bind(): address already in use" msg
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&op, sizeof(op));

	if(bind(fd,(struct sockaddr*)&sr,sizeof(sr)) == -1) {
		perror("bind()");
		return;
	}

	if(listen(fd, 5) == -1) {
		perror("listen()");
		return;
	}

	signal(SIGCHLD, SIG_IGN);
	while(1) {
		if((client = accept(fd,(struct sockaddr *)&cl,&op)) > 0) {
			switch(fork()) {
				case 0: 
					if(!checkkey((uint32_t)cl.sin_addr.s_addr, client))
						init_session(client,(uint32_t)cl.sin_addr.s_addr); 
				break;
				case -1: 
					return; 
				break;
			}
			if(client) close(client);
		}
	}

	return;
}

