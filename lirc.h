#ifndef LIRC_H
#define LIRC_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h> /* Sleep */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#include <fcntl.h>
#include <sys/epoll.h>

#include "util.h"
#include "list.h"

#define MAX_IRC_CMD_LEN 10
#define LIRC_MAX_SERV_EVENTS 128
#define MAX_IRC_MESSAGE_SIZE 512
#define LIRC_SETTINGS_FILE "./settings.cfg"
#define LIRC_VERSION "0.01"
#define LIRC_GITHUB_ADDRESS "https://github.com/TripleChocPi/LIRC/"

typedef enum
{
  FALSE = 0,
  TRUE = 1
} BOOL;

#endif
