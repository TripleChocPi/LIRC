#include "channel.h"
#include "server.h"
#include "client.h"

static int lirc_channel_cmp(const void *channel, const void *c_str);

static LIRCChannelData* lirc_channel_new_metadata()
{
  LIRCChannelData* channel = safe_malloc(sizeof(LIRCChannelData));

  /* Initialize default values for new channel */
  channel->name = NULL;
  channel->client_list = make_empty_list();

  return channel;
}

static int lirc_channel_cmp(const void *channel, const void *c_str)
{
  LIRCChannelData* chan = (LIRCChannelData*)channel;
  char *c = (char *)c_str;

  return strcmp(chan->name, c);
}

void lirc_channel_join(struct LIRCServer_struct* server,
                       struct LIRCClientData_struct* client,
                       char *channel)
{
  char temp[MAX_IRC_MESSAGE_SIZE];
  LIRCChannelData* c = 
    (LIRCChannelData*)find_element(server->channel_list,
                                   channel, lirc_channel_cmp);
  if (c == NULL)
  {
    /* The channel doesn't exist in the list so we
     * will need to create one */
    c = lirc_channel_new_metadata();
    c->name = safe_strdup(channel);
    insert_at_front(server->channel_list, c, sizeof(c));
  }

  /* Add this client to the list */
  insert_at_front(c->client_list, client,
                  sizeof(client));

  /* Add a weak reference to the joined channels list of the client */
  insert_at_front(client->channel_list, c, sizeof(c));

  /* Send the response message indicating the join was successful */
  sprintf(temp, ":%s!~%s@anon.com JOIN %s\r\n", client->nick, client->user, 
          channel);
  send(client->socket, temp, strlen(temp), 0); 
}

void lirc_channel_who(struct LIRCServer_struct* server,
                        struct LIRCClientData_struct* client,
                        char *channel)
{
  int size = 0;
  char temp[MAX_IRC_MESSAGE_SIZE]; 
  LIRCChannelData* c; 
  dlnode_t* client_node;

  c = (LIRCChannelData*)find_element(server->channel_list,
                                   channel, lirc_channel_cmp);
  
  client_node = c->client_list->head;
  memset(temp, '\0', sizeof (char) * MAX_IRC_MESSAGE_SIZE);  

  sprintf(temp, ":lirc.org 353 %s = %s :", client->nick, channel);
  size += strlen(temp);  

  while (client_node != NULL)
  {
    struct LIRCClientData_struct* chan_client = 
      ((struct LIRCClientData_struct*)(client_node->data));

    sprintf(temp + size, "%s ", chan_client->nick);
    size += strlen(chan_client->nick + 1);

    client_node = client_node->next;
  }

  /* Minus 1 to remove the space at the end of the list */
  sprintf(temp + size - 1, "\r\n");
  send(client->socket, temp, strlen(temp), 0);
 
  /* Tell the IRC client that we're done listing client names */
  memset(temp, '\0', sizeof (char) * MAX_IRC_MESSAGE_SIZE); 
  sprintf(temp, ":lirc.org 366 %s %s :End of /NAMES list\r\n",
          client->nick, channel);

  send(client->socket, temp, strlen(temp), 0);
}

void lirc_channel_leave(struct LIRCServer_struct* server,
                        struct LIRCClientData_struct* client,
                        char *channel)
{

}
