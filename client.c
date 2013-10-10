#include "client.h"
#include "server.h"
#include "channel.h"

static int lirc_client_cmp(const void *client, const void *socket);

static LIRCClientData* lirc_client_new_metadata()
{
  LIRCClientData* client = safe_malloc(sizeof(LIRCClientData));

  /* Initialize default values for the new client */
  client->socket = 0;
  client->nick = NULL;
  client->user = "anon";
  client->channel_list = make_empty_list();

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

  insert_at_front(server->client_list, client, sizeof(client));

  /* Send a ping because irssi does a handshake potentially */
  

  /* Send client a welcome and motd */

  /* Send the client the server name */
  sprintf(temp, ":lirc.org 001 %s :Welcome to %s\r\n", nick, 
          settings->server_name);
  send(socket, temp, strlen(temp), 0);

  /* Send the MOTD */
  sprintf(temp, ":lirc.org 372 %s :%s\r\n", nick, settings->motd);
  send(socket, temp, strlen(temp), 0);

  /* End of MOTD */
  sprintf(temp, ":lirc.org 376 %s :%s\r\n", nick, "End of /MOTD command.");
  send(socket, temp, strlen(temp), 0);

  /* Send a PING out */
  sprintf(temp, "PING :%d\r\n", (int)time(NULL));
  send(socket, temp, strlen(temp), 0);

  /* Set the mode of the user */
  sprintf(temp, ":lirc.org 221 %s +i\r\n", nick);
  send(socket, temp, strlen(temp), 0);
  
  sprintf(temp, ":%s!~%s@anon.com MODE %s +i\r\n", client->nick, 
          client->user, nick);
}

void lirc_client_setnick(struct LIRCServer_struct* server, int socket,
                         char* nick)
{
  LIRCClientData* client =
    (LIRCClientData*)find_element(server->client_list, &socket,
                                  lirc_client_cmp);

  if (client != NULL && client->nick != NULL)
  {
    dlnode_t* channel_node;
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

    /* Let every client know of every joined channel that 
     * the nickname has been changed. This code doesn't check for
     * two or more clients in two or more rooms and couple result
     * in duplicates. */
    channel_node = client->channel_list->head;

    while (channel_node != NULL)
    {
      struct LIRCChannelData_struct* c = 
        (struct LIRCChannelData_struct*)channel_node->data;
      dlnode_t* client_node = c->client_list->head;

      while (client_node != NULL)
      {
        LIRCClientData* c_data = 
          (LIRCClientData*)client_node->data;

        send(c_data->socket, temp, strlen(temp), 0);
        client_node = client_node->next;
      }

      channel_node = channel_node->next;
    } 
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

  if (!strcmp(command_string, "JOIN"))
  {
    struct LIRCClientData_struct* client = 
      (struct LIRCClientData_struct*)find_element(server->client_list, 
                                                  &socket,
                                                  lirc_client_cmp);
    lirc_channel_join(server, client, command_args);
    return;
  }

  if (!strcmp(command_string, "WHO"))
  {
    struct LIRCClientData_struct* client = 
      (struct LIRCClientData_struct*)find_element(server->client_list, 
                                                  &socket,
                                                  lirc_client_cmp);
    lirc_channel_who(server, client, command_args);
    return;
  }

  if (!strcmp(command_string, "PING"))
  {
    char temp[MAX_IRC_MESSAGE_SIZE];
    sprintf(temp, ":lirc.org PONG lirc.org :%s\r\n", command_args);
    send(socket, temp, strlen(temp), 0);    
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
