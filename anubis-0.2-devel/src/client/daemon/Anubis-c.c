/*
 * Anubis-c.c
 *
 * Daemon part of the client, this client shall read and write into
 * the /proc/Anubis/ACL file to get stuff from it and write to allow
 * execution or deny it based on the remote ACL
 *
 * Enrique A. Sanchez Montellano
 * nahual@g-con.org
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#include <strings.h>

// Our libraries
#include "client.h"
#include "defs.h"
#include "structs.h"

/* Global Variables */

int fd;
char *password = ANUBIS_PASSWORD;
unsigned long anubis_server;
int remote_auth = 0; //Default to local
int daemonize = 0;
u_long port = PORT;

/* Function definitions */
int md5auth(char *name, u_long victim, char *passwd);

void read_handler() {
  char filename[1024];
  bzero(filename, sizeof(filename));
#ifdef DEBUG
  fprintf(stderr, "Got signal, reading the binary to be executed\n");
#endif
  read(fd, filename, sizeof(filename));
  if(remote_auth == 1) {
    if((md5auth(filename, anubis_server, password)) == 0) {
      write(fd, "EXEC", 4);
      return ;
    }
    else {
      write(fd, "DENIED", 6);
      return;
    }
  }
  /* Local auth */
  else {
    if((md5auth_local(filename)) == 0) {
      write(fd, "EXEC", 4);
      return;
    }
    else {
      write(fd, "DENIED", 6);
      return;
    }
  }
}

void stop_handler() {
#ifdef DEBUG
  fprintf(stderr, "Got SIGSTOP closing device and exitting\n");
#endif
  close(fd);
  exit(0);
}

void hup_handler() {
}

void usage(char *name) {
  fprintf(stderr, "Usage: %s [-hD] [-d <device file>] [-p <password>] [-s <Anubisd Server>] [-c <Config file>]\n", name);
  fprintf(stderr, "\th: This rant\n");
  fprintf(stderr, "\tc: Config file\n");
  fprintf(stderr, "\td: Device file\n");
  fprintf(stderr, "\tp: password\n");
  fprintf(stderr, "\ts: Anubis Server\n");
  fprintf(stderr, "\tD: Daemonize the server\n");
  fprintf(stderr, "Send bugs to anubis@g-con.org\n");
}

int main(int argc, char **argv) {
  int opt;
  char *config_file = CONFIG_FILE;
  char *device_file = DEVICE_FILE;
  char *aserver = ANUBIS_SERVER;

  while((opt = getopt(argc, argv, "hc:d:p:s:D")) != EOF) {
    switch(opt) {
    case 'h':
      usage(argv[0]);
      exit(-1);
      break;
    case 'c':
      config_file = optarg;
      break;
    case 'd':
      device_file = optarg;
      break;
    case 'p':
      password = optarg;
      break;
    case 's':
      aserver = optarg;
      break;
    case 'D':
      daemonize = 1;
      break;
    }
  }

  /* Let's do some sanity check in case something is not defined */
  if((config_file == NULL) || (device_file == NULL) || (password == NULL) || (aserver == NULL)) {
    fprintf(stderr, "Something went terribly wrong!, exiting\n");
    exit(-1);
  }

  anubis_server = resolve(aserver);

  signal(SIGUSR1, read_handler);
  signal(SIGSTOP, stop_handler);
  signal(SIGTERM, stop_handler);
  signal(SIGCHLD, SIG_IGN);

  if(daemonize == 1) {
    switch(fork()) {
    case -1:
      return -1;
      break;
    case 0: //Child
      break;
    default: //Parent
      sleep(1);
      exit(0);
    }
  }
  
  if((fd = open(device_file, O_RDWR)) < 0) {
    perror("open @ device");
    exit(-1);
  }
  
  if((read_the_config_file(config_file)) == -1) {
    exit(-1);
  }

  /* Here we go into an infinite loop untill we are killed */

  while(1) {
    pause();
  }    

  return 0;
}
