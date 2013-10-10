#ifndef CLIENT_H
#define CLIENT_H

#include "lirc.h"
#include "settings.h"

/* Pre-declare structs */
struct LIRCServer_struct;

typedef struct LIRCClientData_struct
{
  int socket;
  char *nick, *user;
} LIRCClientData;

void lirc_client_connect(struct LIRCServer_struct*, int socket, char* nick);
void lirc_client_parse(struct LIRCServer_struct*, LIRCSettings*,
                       int socket, char* command);
void lirc_client_disconnect(struct LIRCServer_struct*, int socket);

LIRCClientData* lirc_client_search(struct LIRCServer_struct*, int socket);
#endif
