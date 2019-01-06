/*
 * resolve.c
 *
 * THIS IS PROPIETARY SOURCE CODE OF KELSI SILER AND G-CON
 * THIS ODE BINDA TO THE MAIN LICENSE IN THE LICENSE FILE 
 * THAT IS CONTAINED WITHIN THIS PROGRAM IF YOU RECIEVED THIS
 * FILE WITHOUT THE LICENSE FILE REFER TO http://anubis.g-con.org/LICENSE
 * FOR THE LICENSE
 *
 * Enrique A. Sanchez Montellano
 * nahual@g-con.org
 */

#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

uint32_t resolve (char *serv) {
  struct sockaddr_in sinn;
  struct hostent *hent;

  hent = gethostbyname (serv);
  if(!hent) return 0;
  bzero ((char *) &sinn, sizeof (sinn));
  memcpy ((char *) &sinn.sin_addr, hent->h_addr, hent->h_length);
  return sinn.sin_addr.s_addr;

}
