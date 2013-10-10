#ifndef SERVER_H
#define SERVER_H

#include "lirc.h"
#include "settings.h"

/* Pre-declare structs */
struct LIRCClientData_struct;

typedef struct LIRCServer_struct
{
  int epoll;

  int l_socket;
  struct sockaddr_in l_socket_addr;

  struct LIRCClientData_struct* head_client;
  int clients;
} LIRCServer;

void lirc_server_init(LIRCServer*, LIRCSettings*);
void lirc_server_main_loop(LIRCServer*, LIRCSettings*);
void lirc_server_close(LIRCServer*);

#endif
