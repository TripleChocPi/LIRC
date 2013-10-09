#ifndef CLIENT_H
#define CLIENT_H

#include "lirc.h"
#include "settings.h"

typedef struct LIRCServer_struct LIRCServer;

typedef struct LIRCClientData_struct
{
  int socket;
  char *nick, *user;
} LIRCClientData;

void lirc_client_meta_init(LIRCServer* server);
void lirc_client_connect(LIRCServer*, int socket);
void lirc_client_parse(LIRCServer*, LIRCSettings*, int socket, char* command);
void lirc_client_disconnect(LIRCServer*, int socket);

LIRCClientData* lirc_client_search(LIRCServer*, int socket);
#endif
