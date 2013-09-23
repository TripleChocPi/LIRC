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
  /* Ask the operating system if we can bind to the port */
  if (bind(server->socket, (const struct sockaddr *)&(server->socket_addr), 
           sizeof (server->socket_addr)) < 0)
    return FALSE;
  else
  {
    make_socket_non_blocking(server->socket);
    return TRUE;
  }
}

BOOL
lirc_server_listen(LIRCServer* server)
{
  /* Set the server up to listen for new connections.
   * SOMAXCONN is the operating system's defined maximum
   * queue length for accepting connections at once */
  return (listen(server->socket, SOMAXCONN) != -1) ? TRUE : FALSE;
}

void
lirc_server_close(LIRCServer* server)
{
  /* Close the socket used to accept connections */
  close(server->socket);
}
