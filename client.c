#include "client.h"
#include "server.h"

static int lirc_client_cmp(const void *client, const void *socket);

static LIRCClientData* lirc_client_new_metadata()
{
  LIRCClientData* client = safe_malloc(sizeof(LIRCClientData));

  /* Initialize default values for the new client */
  client->socket = 0;
  client->nick = NULL;
  client->user = NULL;

  return client;
}

void lirc_client_connect(struct LIRCServer_struct* server, int socket,
                         char* nick)
{
  LIRCClientData* client = lirc_client_new_metadata();
  client->socket = socket;
  client->nick = safe_strdup(nick);

  insert_at_front (server->client_list, client, sizeof(client)); 
}

void lirc_client_parse(struct LIRCServer_struct* server,
                       LIRCSettings* settings, int socket, char* command)
{
  char* saveptr;
  char* c_str;
  BOOL args = FALSE;
  char* command_string = NULL;
  char* command_args = NULL;

  c_str = strtok_r(command, " ", &saveptr);
  /* Command has multiple parameters */
  if (c_str)
  {
    args = TRUE;
    command_string = safe_strdup(c_str);
    c_str = strtok_r(NULL, " ", &saveptr);
    command_args = safe_strdup(c_str);
    printf("Command string = %s\n", command_string);
    printf("Command args = %s\n", command_args);
  }
  else
  {
    /* Command doesn't have arguments. */
    command_string = command;
  }

  if (!strcmp(command_string, "NICK"))
  {
    lirc_client_connect(server, socket, command_args);
    return;
  }

  if (!strcmp(command_string, "QUIT")) 
  {
    lirc_client_disconnect(server, socket);
    return;
  }

  if (args)
  {
    free(command_string);
    free(command_args);
  }
}

void lirc_client_disconnect(struct LIRCServer_struct* server, int socket)
{
  close(socket);
  remove_element(server->client_list, &socket, lirc_client_cmp); 
}

static int lirc_client_cmp(const void *client, const void *socket)
{
  LIRCClientData* client_data = (LIRCClientData*)client;
  int s = *((int *)socket);

  if (client_data->socket == s)
    return 0;
  else
    return -1;
}

LIRCClientData* lirc_client_search(struct LIRCServer_struct* server,
                                   int socket)
{
  return NULL;
}
