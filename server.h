#ifndef SERVER_H
#define SERVER_H

#include "lirc.h"
#include "settings.h"
#include "client.h"

typedef struct LIRCServer_struct
{
  int epoll;

  int l_socket;
  struct sockaddr_in l_socket_addr;

  LIRCClientNode* head_client;
  int clients;
} LIRCServer;

void lirc_server_init(LIRCServer*, LIRCSettings*);
void lirc_server_close(LIRCServer*);

static BOOL lirc_server_socket_init(LIRCServer*, LIRCSettings*);
static BOOL lirc_server_bind(LIRCServer*);
static BOOL lirc_server_listen(LIRCServer*);
static BOOL lirc_server_init_epoll(LIRCServer*);

#endif
