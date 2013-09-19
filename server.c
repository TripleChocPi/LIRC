#include "server.h"

void
lirc_server_init(LIRCServer* server, LIRCSettings* settings)
{
  server->settings = settings;
  server->head_client = NULL;
  server->clients = 0;
}

BOOL
lirc_server_bind(LIRCServer* server)
{
  /* Not implemented */
  return FALSE;
}
