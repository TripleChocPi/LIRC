#include "lirc.h"

typedef struct LIRCClientData_struct
{
  char* client_name;
  int* ip_address;
} LIRCClientData;

typedef struct LIRCClientNode_struct
{
  LIRCClientData* data;
  struct LIRCClient_struct* prev_client;
  struct LIRCClient_struct* next_client;
} LIRCClientNode;
