#include "list.h"

dlist_t *
make_empty_list (void)
{
  dlist_t * empty_list = safe_malloc(sizeof(dlist_t));

  /* Initialize default values for the empty list */
  empty_list->head = NULL;
  empty_list->foot = NULL;
  empty_list->list_len = 0;
  empty_list->size = sizeof(dlist_t);
  return empty_list;
}

void
delete_list (dlist_t *list, void (*destroy_fptr)(void *))
{
  int i;
  dlnode_t * current = list->head;
  dlnode_t * next = NULL;

  /* Iterate through all satellite data and delete it */
  for (i = 0; i < list->list_len; i++)
  {
    next = current->next;
    destroy_fptr(current->data);
    destroy_fptr(current);
    current = next;
  }

  /* Delete the list descriptor */
  destroy_fptr(list);
}

void
insert_at_front (dlist_t *list, void *value, size_t size)
{
  dlnode_t *new = NULL;
  dlnode_t *tmp = NULL;
  new = (dlnode_t *) safe_malloc (sizeof (dlnode_t));
  assert (list != NULL && new != NULL);
  new->data = value;
  new->next = NULL;
  new->prev = NULL;
  if(list->head == NULL)
  {
    list->head = new;
    list->foot = new;
  }
  else
  {
    tmp = list->head;
    list->head = new;
    new->next = tmp;
    tmp->prev = new;
  }
  list->list_len++;
  list->size += size;
  list->size += sizeof (dlnode_t);
}

void *
find_element (dlist_t *list, void *value,
              int (*cmp_fptr) (const void*, const void*))
{
  dlnode_t * current = list->head;

  /* Iterate through the loop and return the value we need if found */
  while (current != NULL)
  {
    if (cmp_fptr(current->data, value) == 0)
      return current->data;

    current = current->next;
  }

  return 0;
}

void*
remove_element(dlist_t* list,void* val,
               int (*cmp_fptr)(const void*,const void*))
{
  dlnode_t *curr = NULL, *prev = NULL;
  void* item;
  curr = list->head;
  while( curr != NULL && cmp_fptr (curr->data,val)!=0 )
  {
    prev = curr;
    curr = curr->next;
  }

  /* not found return NULL */
  if(curr==NULL) return NULL;

  list->size -= curr->size;
  list->size -= sizeof(dlnode_t);

  if(prev==NULL)
  {
    list->head = curr->next;
    if(list->head) list->head->prev = NULL;
    item = curr->data;
    free(curr);
    if(list->head == NULL) list->foot = NULL;
  }
  else
  {
    item = curr->data;
    prev->next = curr->next;
    if(curr->next==NULL) list->foot = prev;
    else
    {
      prev->next->prev = prev;
    }
    free(curr);
  }
  list->list_len--;
  return item;
}


