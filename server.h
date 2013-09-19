#ifndef SERVER_H
#define SERVER_H

#include "lirc.h"
#include "settings.h"
#include "client.h"

typedef struct LIRCServer_struct
{
  LIRCSettings* settings;
  LIRCClientNode* head_client;
  int clients;
} LIRCServer;

void lirc_server_init(LIRCServer*, LIRCSettings*);
BOOL lirc_server_bind(LIRCServer*);

#endif
