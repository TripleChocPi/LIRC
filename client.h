#ifndef CLIENT_H
#define CLIENT_H

#include "lirc.h"
#include "settings.h"
#include "client.h"

typedef struct LIRCClientNode_struct
{
  int socket;
  char *nick, *user;

  struct LIRCClientNode_struct *left_tree;
  struct LIRCClientNode_struct *right_tree;
} LIRCClientNode;

void lirc_client_meta_init();
void lirc_client_connect(int socket, char* nick, char* user);
void lirc_client_disconnect(int socket);
LIRCClientNode* lirc_client_search(int socket);

static void lirc_client_insert(LIRCClientNode *client);
static void lirc_client_delete(LIRCClientNode *client);

#endif
