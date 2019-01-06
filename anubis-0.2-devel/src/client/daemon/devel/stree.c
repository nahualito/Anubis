#include <stdio.h>
#include <netinet/in.h>
#include "structs.h"

int tree_size;

struct _filenode * splay (int i, struct _filenode *t) {
  struct _filenode N, *l, *r, *y;
  if (t == NULL) return t;
  N.left = N.right = NULL;
  l = r = &N;
  
  for (;;) {
    if (i < t->nsize) {
      if (t->left != NULL && i < t->left->nsize) {
	y = t->left; t->left = y->right; y->right = t; t = y;
      }
      if (t->left == NULL) break;
      r->left = t; r = t; t = t->left;
    } else if (i > t->nsize) {
      if (t->right != NULL && i > t->right->nsize) {
	y = t->right; t->right = y->left; y->left = t; t = y;
      }
      if (t->right == NULL) break;
      l->right = t; l = t; t = t->right;
    } else break;
  }
  l->right=t->left; r->left=t->right; t->left=N.right; t->right=N.left;
  return t;
}

struct _filenode * create_filenode(uint32_t serverkey, char *fname, char *fingerprint) {
  struct _filenode *newNode;

  newNode = (struct _filenode *)malloc(sizeof(struct _filenode));
  newNode->serverkey = serverkey;
  newNode->nsize = strlen(fname);
  newNode->lfname = fname;
  newNode->fingerprint = fingerprint;
  newNode->left = NULL;
  newNode->right = NULL;

  return newNode;
}

struct _filenode * insert_filenode(struct _filenode *new, struct _filenode *t) {
  int i = new->nsize;

  if (new == NULL) {
    return NULL;
  }
  if (t == NULL) {
    new->left = new->right = NULL;
    tree_size = 1;
    return new;
  }
  t = splay(i,t);
  if (i < t->nsize) {
    new->left = t->left;
    new->right = t;
    t->left = NULL;
    tree_size ++;
    return new;
  } else if (i > t->nsize) {
    new->right = t->right;
    new->left = t;
    t->right = NULL;
    tree_size++;
    return new;
  } else { 
    // already there
    return t;
  }
  return NULL;
}

int main(void) {
  return 0;
}
