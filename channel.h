#ifndef CHANNEL_H
#define CHANNEL_H

#include "lirc.h"
#include "settings.h"

/* Forward declarations */
struct LIRCServer_struct;
struct LIRCClientData_struct;

typedef struct LIRCChannelData_struct
{
  char* name;
  dlist_t* client_list;
} LIRCChannelData;

void lirc_channel_join(struct LIRCServer_struct*,
                       struct LIRCClientData_struct*,
                       char *channel);

void lirc_channel_who(struct LIRCServer_struct*,
                       struct LIRCClientData_struct*,
                       char *channel);

void lirc_channel_leave(struct LIRCServer_struct*,
                        struct LIRCClientData_struct*,
                        char *channel);

void lirc_channel_message(struct LIRCServer_struct* server,
                        struct LIRCClientData_struct* client,
                        char *channel, char *message);


/* :penzrgb!~penzrgb@fddfsdfsdsffd.au JOIN #quake
:portlane.se.quakenet.org 353 penzrgb = #quake :penzrgb lol
:portlane.se.quakenet.org 366 penzrgb #quake :End of /NAMES */

#endif
