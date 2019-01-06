/*
  test.c

  test client for the server

  Nahual
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <time.h>


unsigned long resolve (char *serv) {
  struct sockaddr_in sinn;
  struct hostent *hent;

  hent = gethostbyname (serv);
  bzero ((char *) &sinn, sizeof (sinn));
  memcpy ((char *) &sinn.sin_addr, hent->h_addr, hent->h_length);
  return sinn.sin_addr.s_addr;

}

int connie(u_long *victim) {
  int sockfd;
  struct sockaddr_in hostaddr;
  
  if((sockfd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    printf("Cannot allocate socket! \n");
    exit(-1);
  }
  
  hostaddr.sin_port = htons (4005);
  hostaddr.sin_addr.s_addr = victim;
  hostaddr.sin_family = AF_INET;
  
  if( (connect(sockfd, (struct sockaddr *) &hostaddr, sizeof(hostaddr))) < 0 ) {
    printf("Error conecting! \n");
    exit(-1);
  }
  else {
    return sockfd;
  }
  
}

int check_validity(u_long victim, char *name, char **envy) {
  int socky = 0, i;
  fd_set rfds;
  struct timeval timer;
  int retval;
  char tmp[255], *sendshit;
  unsigned char *fingerprint;
  char passwd[]="\tprueba\n";

  socky = connie(victim);

  FD_ZERO(&rfds);
  FD_SET(socky, &rfds);
  timer.tv_sec = 5;
  timer.tv_usec = 0;

  retval = select (socky + 1, NULL, &rfds, NULL, &timer);

  fingerprint = (unsigned char *)MDFile(name);
  sendshit = (unsigned char *)malloc(1024);

  bzero(tmp, sizeof(tmp));
  if (retval) {
    if(read(socky, tmp, sizeof(tmp)) != -1) {
      if (1 == 1) {
	bzero(tmp, sizeof(tmp));
	if(write(socky, passwd, strlen(passwd)) < strlen(passwd)){
		printf("error while writting to socket (2)\n");
		exit(-1);
	}
	printf("[ + ] Connected and sent the passwd\n");
read_again:
	if(read(socky, tmp, sizeof(tmp)) > 0) {
		if(strncmp(tmp, "AG", strlen("AG"))) {
			printf("Shit access denied\n");
			printf("%s\n", tmp);
			return 0;
		}
	}
	else {
		goto read_again;
	}
//	printf("DEBUG %s\n", tmp);
	bzero(tmp, sizeof(tmp));
	if(read(socky, tmp, sizeof(tmp)) != -1) {
		bzero(sendshit, 1024);
//		printf("DEBUG2 %s\n", tmp);
		printf("[ + ] Writting %s as name and ", name);
		MDPrint(fingerprint);
		printf(" as fingeprint\n");
		sprintf(sendshit, "\t%s:%s\n", name, fingerprint);
		write(socky, sendshit, strlen(sendshit));
		bzero(sendshit, 1024);
	}
	bzero(tmp, sizeof(tmp));
	if(read(socky, tmp, sizeof(tmp)) != -1) {
//		printf("debug2 %s\n");
	  if(!strncmp("OK", tmp, strlen("OK"))) {
//		  printf("%s\n", tmp);
		  free(sendshit);
	    //	    execl(name, name, NULL);
	    execve(envy[0], envy, NULL);
	  }
	  else {
		  printf("[ + ] Execution denied by Anubis server\n");
		  exit(-1);
	  }
	}
      }
      else {
	      printf("This is not an anubis server\n");
      }
    }
  }
  free(sendshit);

  return 0;
}

int main(int argc, char **argv) {
  char *envy[255];
  int i = 2;

  if(argc < 3) {
	  printf("Usage: %s <Anubis server> <full path binary> [arguments]\n");
	  printf("\t-Arguments are optional\n");
	  printf("bugs to anubis@g-con.org\n");
	  exit(-1);
  }

  printf("[ + ] Anubis test file instalation program ... \n");

  while(envy[i] != NULL) {
    envy[i - 2] = argv[i];
    i++;
  }
  
  check_validity(resolve(argv[1]), argv[2], envy);

  return 0;
}
