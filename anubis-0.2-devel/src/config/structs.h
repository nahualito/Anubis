/*
 *  structs.h
 *
 * This part of Anubis reads th econfig file wich is a 
 * random accessed file.
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

#ifndef _STRUCTS_H
#define _STRUCTS_H

struct _filenode {
  uint32_t serverkey;
  char *sfname;
  char *lfname;
  unsigned char *fingerprint;
  struct _filenode *next;
};

struct _cfilenode {
	uint32_t serverkey;
	char *sfname;
	char *lfname;
	unsigned char fingerprint[16];
};

struct _list {
  struct _filenode *first_node;
  struct _filenode *last_node;
} list;

#endif
