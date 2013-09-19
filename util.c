/* Useful utility functions taken from a previous assignment's
 * skeleton for Algorithms and Analysis at RMIT University.
 * Attribution: Shane Culpepper */
#include "util.h"

void *
safe_malloc (size_t size)
{
  void * ptr = malloc(size);
  if (ptr == NULL)
  {
    errno = EXIT_FAILURE;
    perror("Attempt to alloc memory failed (malloc)");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void *
safe_realloc (void *old_mem, size_t new_size)
{
  void * ptr = realloc(old_mem, new_size);
  if (ptr == NULL)
  {
    errno = EXIT_FAILURE;
    perror("Attempt to alloc memory failed (realloc)");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

char *
safe_strdup (const char *str)
{
  char *ptr = NULL;

  if (str == NULL)
  {
    errno = EXIT_FAILURE;
    perror("Attempt to duplicate string failed (strdup)");
    exit(EXIT_FAILURE);
  }

  /* Allocate space for our new duplicate. Add 1 to
   * allow space for null termination. */
  ptr = safe_malloc((strlen(str) + 1) * sizeof(char));

  /* Avoiding the use of strdup as it is not a part of the standard C library
   * as indicated by the compiler warnings when trying to wrap it.
   * Instead we're using strcpy. Used man pages to ensure there were no
   * edge cases */
  /* strcpy(ptr, str); */
  memcpy(ptr, str, (strlen(str) + 1) * sizeof(char));
  return ptr;
}

int
cmp_int (const void *a, const void *b)
{
  int a_val = *((int*)a);
  int b_val = *((int*)b);

  if (a_val == b_val)
    return 0;
  else
    return a_val - b_val;
}
