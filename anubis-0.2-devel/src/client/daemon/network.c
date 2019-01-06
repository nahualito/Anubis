/*
 * network.c
 *
 * Network part of Anubis server, this and md5auth do all the work
 *
 * Enrique A. Sanchez Montellano
 * nahual@g-con.org
 */

#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

int connex(u_long victim, u_long port) {
  int sockfd;
  struct sockaddr_in hostaddr;
  
  if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    fprintf(stderr, "Cannot allocate socket\n");
    return -1;
  }
  
  hostaddr.sin_port = htons(port);
  hostaddr.sin_addr.s_addr = victim;
  hostaddr.sin_family = AF_INET;
  
  if((connect(sockfd, (struct sockaddr *)&hostaddr, sizeof(hostaddr))) < 0) {
    fprintf(stderr, "Error connecting\n");
    return -1;
  }
  else {
    return sockfd;
  }
}

