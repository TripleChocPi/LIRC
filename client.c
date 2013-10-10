#include "client.h"

static void lirc_client_insert(LIRCClientData* client);
static void lirc_client_delete(LIRCClientData* client);

void lirc_client_meta_init(struct LIRCServer_struct* server)
{

}

void lirc_client_connect(struct LIRCServer_struct* server, int socket)
{

}

void lirc_client_parse(struct LIRCServer_struct* server, 
                       LIRCSettings* settings, int socket, char* command)
{
  lirc_client_insert(NULL);
}

void lirc_client_disconnect(struct LIRCServer_struct* server, int socket)
{
  lirc_client_delete(lirc_client_search(server, socket));
}

LIRCClientData* lirc_client_search(struct LIRCServer_struct* server, 
                                   int socket)
{
  return NULL;
}

static void lirc_client_insert(LIRCClientData* client)
{

}

static void lirc_client_delete(LIRCClientData* client)
{

}
