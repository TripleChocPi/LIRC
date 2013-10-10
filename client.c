#include "client.h"
#include "server.h"

static int lirc_client_cmp(const void *client, const void *socket);

static LIRCClientData* lirc_client_new_metadata()
{
  LIRCClientData* client = safe_malloc(sizeof(LIRCClientData));

  /* Initialize default values for the new client */
  client->socket = 0;
  client->nick = NULL;
  client->user = "anon";

  return client;
}

void lirc_client_connect(struct LIRCServer_struct* server,
                         LIRCSettings* settings, int socket,
                         char* nick)
{
  char temp[MAX_IRC_MESSAGE_SIZE];
  LIRCClientData* client = lirc_client_new_metadata();
  client->socket = socket;
  client->nick = safe_strdup(nick);

  insert_at_front (server->client_list, client, sizeof(client));

  /* Send client a welcome and motd */

  /* Send the client the server name */
  sprintf(temp, "NOTICE : Welcome to %s\r\n", settings->server_name);
  send(socket, temp, strlen(temp), 0);

  sprintf(temp, "NOTICE : %s\r\n", settings->motd);
  send(socket, temp, strlen(temp), 0);
}

void lirc_client_setnick(struct LIRCServer_struct* server, int socket,
                         char* nick)
{
  LIRCClientData* client =
    (LIRCClientData*)find_element(server->client_list, &socket,
                                  lirc_client_cmp);

  if (client != NULL && client->nick != NULL)
  {
    char temp[MAX_IRC_MESSAGE_SIZE];

    sprintf(temp, ":%s!~%s@anon.com NICK :", client->nick, client->user);

    printf("Nickname changed from %s ", client->nick);
    free(client->nick);
    client->nick = safe_strdup(nick);
    printf("to %s.\n", client->nick);

    /* Send an acknowledgement response to the client to say
     * everything went ok. */
    sprintf(temp + strlen(temp), "%s\r\n", client->nick);
    send(socket, temp, strlen(temp), 0);
  }
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
    if (find_element(server->client_list, &socket, lirc_client_cmp) == NULL)
      lirc_client_connect(server, settings, socket, command_args);
    else
      lirc_client_setnick(server, socket, command_args);
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
