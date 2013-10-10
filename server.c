#include "server.h"
#include "client.h"

static BOOL lirc_server_socket_init(LIRCServer* server,
                                    LIRCSettings* settings);
static BOOL lirc_server_bind(LIRCServer* server);
static BOOL lirc_server_listen(LIRCServer* server);
static BOOL lirc_server_init_epoll(LIRCServer* server);

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
  if(!(lirc_server_init_epoll(lirc_server)))
  {
    fprintf(stderr, "LIRC could not initiate an epoll instance.\n");
    lirc_server_close(lirc_server);
    lirc_settings_destroy(lirc_settings);
    exit(EXIT_FAILURE);
  }

  /* Set up the client list to be added and removed from */
  lirc_server->clients = 0;
  lirc_server->client_list = make_empty_list(); 

  printf("Server socket bound and listening on port %d.\n",
         lirc_settings->port);
}

void
lirc_server_main_loop(LIRCServer* server, LIRCSettings* settings)
{
  int i, event_count;
  struct epoll_event events[LIRC_MAX_SERV_EVENTS];

  /* Handle multiple events at once */
  memset(events, '\0', sizeof (struct epoll_event) * LIRC_MAX_SERV_EVENTS);

  /* Much of the code below is derived from
   * https://banu.com/blog/2/how-to-use-epoll-a-complete-example-in-c/
   */
  event_count = epoll_wait(server->epoll, events, LIRC_MAX_SERV_EVENTS, -1);
  for (i = 0; i < event_count; i++)
  {
    if ((
          (events[i].events & EPOLLERR) ||
          (events[i].events & EPOLLHUP) ||
          (!(events[i].events & EPOLLIN))
        )
        && (events[i].data.fd != server->l_socket))
    {
      /* An error has occured on this socket, so we will close it. */
      fprintf(stderr, "Error on a client's socket. Connection closed.");

      /* TODO: Add a client disconnect event for all other clients. */
      close(events[i].data.fd);
      continue;
    }
    else if (events[i].data.fd == server->l_socket)
    {
      /* The listening socket has recieved a notification.
       * There are one or more incoming connections */
      while (TRUE)
      {
        struct epoll_event event;
        struct sockaddr in_addr;
        socklen_t in_len;
        int socket;

        in_len = sizeof (in_addr);
        socket = accept(server->l_socket, &in_addr, &in_len);
        if (socket == -1)
        {
          if ((errno != EAGAIN) && (errno != EWOULDBLOCK))
            perror ("Error accepting connection");

          /* All incoming connections have been processed. */
          break;
        }

        /* Make all I/O with the new client non-blocking. */
        make_socket_non_blocking(socket);

        /* Register the new client with epoll */
        event.data.fd = socket;
        event.events = EPOLLIN | EPOLLET;

        if ((epoll_ctl(server->epoll, EPOLL_CTL_ADD, socket, &event)) == -1)
        {
          perror("epoll_ctl add client error");
          lirc_server_close(server);
          lirc_settings_destroy(settings);
          exit(EXIT_FAILURE);
        }

        printf("Client connected.\n");
        /* TODO: Add client object to server linked list */
      }
    }
    else
    {
      BOOL disconnect = FALSE;
      while (TRUE)
      {
        ssize_t count;
        char buffer[MAX_IRC_MESSAGE_SIZE];
        char temp[MAX_IRC_MESSAGE_SIZE];
        char* saveptr;
        char* command;

        memset(buffer, '\0', sizeof(char) * MAX_IRC_MESSAGE_SIZE);
        count = read(events[i].data.fd, buffer, sizeof(buffer));
        if (count == -1)
        {
          /* EAGAIN is good, it means that we have read all the data. */
          if (errno != EAGAIN)
          {
            perror("Read error");
            disconnect = TRUE;
          }
          break;
        }
        else if (count == 0)
        {
          /* End of file, remote has closed the connection. */
          disconnect = TRUE;
          break;
        }

        /* Need to use reentrant version of strtok because
         * it uses a static buffer and is therefore not thread-safe. */
        command = strtok_r(buffer, "\r\n", &saveptr);

        while (command)
        {
          printf("Message recieved: %s\n", command);
          lirc_client_parse(server, settings, events[i].data.fd, command);
          command = strtok_r(NULL, "\r\n", &saveptr);
        }

        sprintf(temp, "NOTICE :*** %s\r\n", settings->server_name);
        /* Send the client the message of the day */
        send(events[i].data.fd, temp, strlen(temp), 0);

      }

      if (disconnect)
      {
        printf("Disconnected a client.\n");
        close(events[i].data.fd);
      }
    }
  }
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
