#ifndef SERVER_H
#define SERVER_H

#include "lirc.h"
#include "settings.h"

typedef struct LIRCClientData_struct LIRCClientData;

typedef struct LIRCServer_struct
{
  int epoll;

  int l_socket;
  struct sockaddr_in l_socket_addr;

  LIRCClientData* head_client;
  int clients;
} LIRCServer;

void lirc_server_init(LIRCServer*, LIRCSettings*);
void lirc_server_main_loop(LIRCServer*, LIRCSettings*);
void lirc_server_close(LIRCServer*);

#endif
