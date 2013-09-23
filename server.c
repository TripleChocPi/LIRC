#include "server.h"

void
lirc_server_init(LIRCServer* lirc_server, LIRCSettings* lirc_settings)
{
  /* Populate the settings struct from the settings file. */
  if(!(lirc_init_settings(lirc_settings, LIRC_SETTINGS_FILE)))
  {
    lirc_settings_destroy(lirc_settings);
    exit(EXIT_FAILURE);
  }

  printf("Settings loaded from %s.\n", LIRC_SETTINGS_FILE);

  /* Initialize the server and set it up to point to the
   * port specified in the settings file. */
  if(!(lirc_server_socket_init(lirc_server, lirc_settings)))
  {
    perror("Creating the server socket failed");
    lirc_settings_destroy(lirc_settings);
  }

  printf("Server socket created...\n");

  /* Ask the operating system if we can bind to the port specified */
  if(!(lirc_server_bind(lirc_server)))
  {
    fprintf(stderr, "LIRC could not bind to the port specified.\n");
    lirc_settings_destroy(lirc_settings);
    exit(EXIT_FAILURE);
  }

  /* Start listening for incoming connections */
  if(!(lirc_server_listen(lirc_server)))
  {
    fprintf(stderr, "LIRC could not listen on the socket specified.\n");
    lirc_server_close(lirc_server);
    lirc_settings_destroy(lirc_settings);
    exit(EXIT_FAILURE);
  }

  /* Initialize epoll */
  if(!(lirc_server_listen(lirc_server)))
  {
    fprintf(stderr, "LIRC could not initiate an epoll instance.\n");
    lirc_server_close(lirc_server);
    lirc_settings_destroy(lirc_settings);
    exit(EXIT_FAILURE);
  }

  printf("Server socket bound and listening on port %d.\n",
         lirc_settings->port);
}

void
lirc_server_close(LIRCServer* server)
{
  /* Close the socket used to accept connections */
  close(server->l_socket);
}

static BOOL
lirc_server_socket_init(LIRCServer* server, LIRCSettings* settings)
{
  /* Initialize values */
  server->head_client = NULL;
  server->clients = 0;

  /* Set up the server socket */
  if ((server->l_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    return FALSE;

  /* Set up the listening socket address information
   * Code derived from glibc manual:
   * http://www.gnu.org/software/libc/manual/html_node/Inet-Example.html */
  server->l_socket_addr.sin_family = AF_INET;
  server->l_socket_addr.sin_port = htons(settings->port);
  server->l_socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  return TRUE;
}

static BOOL
lirc_server_bind(LIRCServer* server)
{
  /* Ask the operating system if we can bind to the port */
  if (bind(server->l_socket, (const struct sockaddr *)&(server->l_socket_addr),
           sizeof (server->l_socket_addr)) < 0)
    return FALSE;
  else
  {
    make_socket_non_blocking(server->l_socket);
    return TRUE;
  }
}

static BOOL
lirc_server_listen(LIRCServer* server)
{
  /* Set the server up to listen for new connections.
   * SOMAXCONN is the operating system's defined maximum
   * queue length for accepting connections at once */
  return (listen(server->l_socket, SOMAXCONN) != -1) ? TRUE : FALSE;
}

static BOOL
lirc_server_init_epoll(LIRCServer* server)
{
  struct epoll_event event;

  /* Create an epoll instance */
  server->epoll = epoll_create1(0);

  if (server->epoll == -1)
    return FALSE;

  /* Register the listening socket with epoll */
  event.data.fd = server->l_socket;
  event.events = EPOLLIN | EPOLLET;

  return (epoll_ctl(server->epoll, EPOLL_CTL_ADD,
                    server->l_socket, &event) != -1)
         ? TRUE : FALSE;
}
