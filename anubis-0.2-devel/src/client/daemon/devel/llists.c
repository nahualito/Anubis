/*
 * llists.c
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
 * Notes:
 *
 * May 26 2003:
 * 	- Changed the search_node() for search_node2() wich now checks for
 * 	the name of the binary and the for the signature. --Nahual
 *
 * Enrique A. Sanchez Montellano
 * nahual@g-con.org
 */


#include <stdio.h>
#include <netinet/in.h>
#include "structs.h"

int init_list(void) {
  list.first_node = NULL;
  list.last_node = NULL;
}

int add_filenode(struct _filenode *filenode) {

  if(list.first_node == NULL) {
    list.first_node = list.last_node = filenode;
    list.first_node->next = NULL;
    return 0;
  }
  else {
#ifdef DEBUG
    fprintf(stderr, "list is not null .. cool\n");
#endif
    list.last_node->next = filenode;
    list.last_node = filenode;
    list.last_node->next = NULL;
    return 0;
  }
  return -1;
}

int print_filenodes(void) {
  struct _filenode *filer = list.first_node;

  if(filer == NULL) {
    fprintf(stderr, "The list is empty!\n");
    return 0;
  }

  do {
    printf("Node with serverkey = %u, sfname = %s lfname = %s and fingerprint ", filer->serverkey, filer->sfname, filer->lfname);
    //	MDPrint(filer->fingerprint);
    printf("\n");
    filer = filer -> next;
  } while(filer != NULL);
  return 0;
}

struct _filenode * create_filenode(uint32_t serverkey, char *fname, char *fingerprint, unsigned long read_permission, unsigned long write_permission, unsigned long exec_permission) {
  struct _filenode *newNode;

  newNode = (struct _filenode *)malloc(sizeof(struct _filenode));
  newNode->serverkey = serverkey;
  newNode->sfname = fname;
  newNode->fingerprint = fingerprint;

  return newNode;
}

/* 
 * Obsolete by search_node2() here for historical purposes
 */

int search_node(char *filename, char *digest) {
  struct _filenode *filer = list.first_node;

#ifdef LOCAL
  unsigned char *digest;
  
  digest = (unsigned char *)malloc(16);
  
  if(filer == NULL) {
    fprintf(stderr, "The list is empty!\n");
    return -1;
  }
  
  digest = (unsigned char *)MDFile(filename);
#endif
  
  do {
    //    MDPrint(digest);
    printf(" == ");
    //    MDPrint(filer->fingerprint);
    printf("\n");
    if(!(strncmp(digest, filer->fingerprint, 16))) {
      return 0;
    }
    filer = filer -> next;
  } while(filer != NULL);
  return -1;
}

int search_node2(char *remote_file_name, char *digest) {
  struct _filenode *filer = list.first_node;
  
  if(remote_file_name[strlen(remote_file_name) - 1] == '\n') {
    remote_file_name[strlen(remote_file_name) - 1] = '\0';
  }
  
  if(digest[strlen(digest) - 1] == '\n') {
    digest[strlen(digest) - 1] = '\0';
  }
  
  // No weird stuff as the key
  if(strlen(digest) != 16) {
    return 1;
  }
  
  do {
    // This should be fast
    if(strlen(remote_file_name) == strlen(filer->sfname)) {
      if(!(strncmp(remote_file_name, filer->sfname, strlen(filer->sfname)))) {
	// Ok now we check fastly for the md5 fingerprint
	if(!(strncmp(digest, filer->fingerprint, 16))) {
	  return 0;
	}
      }
    }
    if(strlen(remote_file_name) == strlen(filer->lfname)) {
      if(!(strncmp(remote_file_name, filer->lfname, strlen(filer->lfname)))) {
	if(!(strncmp(digest, filer->fingerprint, 16))) {
	  return 0;
	}
      }
    }
    filer = filer->next;
  } while(filer != NULL);
  
  return 1;
}

int main(void) {
  return 0;
}
