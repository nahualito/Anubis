/*
 * md5parse.c
 *
 * This file will parse and get the md5 of the file it gets
 * from there it will send to the server and get the response
 *
 *
 * CHANGES:
 *         - Changed due to the overhead of calling another function
 *           for the auth, so we do almost everything here -- Nahual
 *
 * Enrique A. Sanchez Montellano
 * nahual@g-con.org
 */

#include <time.h>
#include <fcntl.h>
#include "defs.h"
#include "md5/md5c.c"
#include "md5/mddriver.c"

extern int daemonize;
extern int remote_auth;
extern u_long port;

int md5auth(char *name, u_long victim, char *passwd) {
  FILE *inFile;
  int sockfd, i;
  struct timeval timer;
  int retval;
  fd_set rdfs;
  char tmp[256], *sendbuffer;
  
  
  if((inFile = fopen(name, "r")) == NULL) {
    if(daemonize) {
      fprintf(stderr, "Could not open the file, maybe the file is not there?\n");
    }
    return -1;
  }
  
  /* So we got here, wich means the file is correct, we close it and then MDFile it */
  fclose(inFile);

  sockfd = connex(victim, port);
  
  if(sockfd == -1) {
    return -1;
  }
  
  FD_ZERO(&rdfs);
  FD_SET(sockfd, &rdfs);
  timer.tv_sec = 5;
  timer.tv_usec = 0;
  
  retval = connect(sockfd + 1, NULL, &rdfs, NULL, &timer);
  sendbuffer = (unsigned char *)malloc(1024);
  
  bzero(tmp, sizeof(tmp));
  if(retval) {
    if(read(sockfd, tmp, sizeof(tmp)) > -1) {
      if(!strncmp(tmp, "1      Hello there, this is anubis speaking.", strlen("1       Hello there, this is anubis speaking.")) == 0) {
	bzero(tmp, sizeof(tmp));
	if(write(sockfd, passwd, strlen(passwd)) < strlen(passwd)) {
	  fprintf(stderr, "Error writting to socket\n");
	  return -1;
	}
      read_again:
	if(read(sockfd, tmp, sizeof(tmp)) > 0) {
	  if(strncmp(tmp, AUTH_GRANTED, strlen(AUTH_GRANTED))) {
	    fprintf(stderr, "Access Denied\n");
	    return -1;
	  }
	}
	else {
	  goto read_again;
	}
	
	bzero(tmp, sizeof(tmp));
	if(read(sockfd, tmp, sizeof(tmp)) > 0) {
	  snprintf(sendbuffer, 1024, "\t%s:%s\n", name, MDFile(name));
	  write(sockfd, sendbuffer, strlen(sendbuffer));
	  bzero(sendbuffer, 1024);
	}
	bzero(tmp, sizeof(tmp));
	if(read(sockfd, tmp, sizeof(tmp)) > 0) {
	  if(!strncmp("OK", tmp, strlen("OK"))) {
	    printf("Granting exec on md5auth\n");
	    close(sockfd);
	    free(sendbuffer);
	    return 0;
	  }
	}
      }
    }
  }
  free(sendbuffer);
  close(sockfd);
  
  return -1;
}

int md5auth_local(char *name) {
  FILE *inFile;

  if((inFile = fopen(name, "r")) == NULL) {
    if(!daemonize) {
      perror("fopen");
    }
    return -1;
  }

  fclose(inFile);

  if((search_node2(name, MDFile(name))) != 0) {
    return -1;
  }

  return 0;
}
