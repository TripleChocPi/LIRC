#include "server.h"

BOOL
lirc_server_init(LIRCServer* server, LIRCSettings* settings)
{
  /* Initialize values */
  server->settings = settings;
  server->head_client = NULL;
  server->clients = 0;

  /* Set up the server socket */
  if ((server->socket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    return FALSE;

  /* Set up the socket address information
   * Code derived from glibc manual:
   * http://www.gnu.org/software/libc/manual/html_node/Inet-Example.html */
  server->socket_addr.sin_family = AF_INET;
  server->socket_addr.sin_port = htons(settings->port);
  server->socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  return TRUE;
}

BOOL
lirc_server_bind(LIRCServer* server)
{
  /* Not implemented */
  return FALSE;
}
