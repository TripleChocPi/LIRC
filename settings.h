#ifndef SETTINGS_H
#define SETTINGS_H

#include "lirc.h"
#define LIRC_SETTINGS_BUFFER_SIZE 1024

typedef enum
{
  LIRC_SETTINGS_LINE_PORT = 0,
  LIRC_SETTINGS_LINE_NAME = 1,
  LIRC_SETTINGS_LINE_MOTD = 2
} LIRC_SETTINGS_LINE;

typedef struct LIRCSettings_struct
{
  int port;
  char* server_name;
  char* motd;
} LIRCSettings;

BOOL lirc_init_settings(LIRCSettings*, const char*);
void lirc_settings_destroy(LIRCSettings*);

#endif
