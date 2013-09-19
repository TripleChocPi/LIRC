#ifndef SERVER_H
#define SERVER_H

#include "lirc.h"
#include "settings.h"
#include "client.h"

typedef struct LIRCServer_struct
{
  int socket;
  struct sockaddr_in socket_addr;

  LIRCSettings* settings;

  LIRCClientNode* head_client;
  int clients;
} LIRCServer;

BOOL lirc_server_init(LIRCServer*, LIRCSettings*);
BOOL lirc_server_bind(LIRCServer*);
void lirc_server_close(LIRCServer*);

#endif
