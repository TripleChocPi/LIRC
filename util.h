/* Useful utility functions taken from a previous assignment's
 * skeleton for Algorithms and Analysis at RMIT University.
 * Attribution: Shane Culpepper */
#ifndef _LIBUTIL_H_
#define _LIBUTIL_H_

#include "lirc.h"

/* Create a new block of memory and do vital error checking.
 * The return value should be free'd by the calling function.
 */
void *safe_malloc (size_t size);

/* Resize a pre-existing block of memory and do vital error checking.
 * The return value should be free'd by the calling function.
 */
void *safe_realloc (void *old_mem, size_t new_size);

/* Take a current string and create a new copy. The
 * return value should be free'd by the calling function.
 */
char *safe_strdup (const char *str);

/* Compare two integers. Return 0 if equal, negative if a < b, positive
 * if a > b.
 */
int cmp_int (const void *a, const void *b);

#endif

