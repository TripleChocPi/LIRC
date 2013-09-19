#ifndef LIRC_H
#define LIRC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "util.h"

#define LIRC_SETTINGS_FILE "./settings.cfg"
#define LIRC_VERSION "0.01"
#define LIRC_GITHUB_ADDRESS "https://github.com/TripleChocPi/LIRC/"

typedef enum
{
  FALSE = 0,
  TRUE = 1
} BOOL;

#endif