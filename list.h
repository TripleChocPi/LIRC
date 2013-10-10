/*** Utility Types ***/
#ifndef _LIBLIST_H_
#define _LIBLIST_H_

#ifdef __cplusplus
extern "C" {
#endif /** cplusplus **/

#include "lirc.h"

typedef struct dlnode dlnode_t;

struct dlnode
{
  int size;
  void *data;    /* A pointer to a generic satellite data payload */
  dlnode_t *next; /* A pointer to the next item in the list */
  dlnode_t *prev; /* A pointer to the previous item in the list */
};

typedef struct
{
  dlnode_t *head; /* A pointer to the head node of the list */
  dlnode_t *foot; /* A pointer to the foot node of the list */
  int list_len;  /* Total number of items in the list */
  int size;      /* Size of the list in bytes */
} dlist_t;

/* Initialize a new singly linked list */
dlist_t *make_empty_list (void);
/* Free a list and all satellite data */
void delete_list (dlist_t *list, void (*destroy_fptr)(void *));
/* Insert a new item */
void insert_at_front (dlist_t *list, void *value, size_t size);
/* Delete item */
void* remove_element(dlist_t*,void *value,
                     int (*cmp_fptr)(const void*,const void*));

/* Find an item in the list, return NULL if not found */
void *find_element (dlist_t *list, void *value,
                    int (*cmp_fptr)(const void *, const void *));

#ifdef __cplusplus
}
#endif /** cplusplus **/

#endif /** _LIBLIST_H_ **/

