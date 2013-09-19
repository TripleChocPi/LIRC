#include <stdio.h>
#include <stdafx.h>

/*****************/
/* LIRC Settings */
/*****************/
enum LIRC_SETTINGS_LINE
{
  LIRC_SETTINGS_LINE_PORT = 0,
  LIRC_SETTINGS_LINE_NAME = 1,
  LIRC_SETTINGS_LINE_MOTD = 2
};

typedef struct LIRCSettings_struct
{
  int port;
  char* server_name;
  char* motd;
} LIRCSettings;

/*****************/
/*  LIRC Client  */
/*****************/
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

/*****************/
/*  LIRC Server  */
/*****************/
typedef struct LIRCServer_struct
{
  LIRCSettings* settings;
  LIRCClientNode* head_client;
  int clients;
} LIRCServer;

/*****************/
/*   LIRC Main   */
/*****************/
void display_welcome(void);
bool lirc_init_settings(LIRCSettings*, const char*);

/* LIRC Main functions */
int 
main(int argc, char *argv[])
{
  LIRCSettings lirc_settings;
  LIRCServer lirc_server;

  /* Display a welcome message to the console. */
  display_welcome();

  /* Populate the settings struct from the settings file. */
  if(!(lirc_init_settings(&lirc_settings, LIRC_SETTINGS_FILE)))
  {
    fprintf(stderr, "LIRC could not read the settings file.\n");
    fprintf(stderr, "Please check that the settings file is readable.");
    return EXIT_FAILURE;
  }

  /* Initialize the server and bind to the port specified
   * in the settings file. */ 
  lirc_server_init(&lirc_server, &lirc_settings); 
  if(!(lirc_server_bind(&lirc_server)))
  {
    fprintf(stderr, "LIRC could not bind to the port specified.\n");
    fprintf(stderr, "Please check that the settings file is correct.");
    return EXIT_FAILURE;
  }
  
  /* Enter the main loop of the server application */

  return EXIT_SUCCESS;
}

void
display_welcome(void)
{
  printf("LIRC Server v%s starting...\n", LIRC_VERSION);
  printf("Developed by TripleChocPi 2013.\n\n");
  printf("LIRC is an open-source project released under the GPL v3.\n");
  printf("Pull requests are welcome. For more information on LIRC's\n");
  printf("design philosophy and goals please visit our GitHub repo\n");
  printf("at %s\n\n", LIRC_GITHUB_ADDRESS);
}

/* LIRC Settings file functions */
bool
lirc_init_settings(LIRCSettings* settings, const char* file_name)
{
  FILE* fp = NULL;
  LIRC_SETTINGS_LINE line = 0;
  char buffer[LIRC_SETTINGS_BUFFER_SIZE];
  
  fp = fopen(file_name);

  /* There was an error opening the settings file. */
  if (fp == NULL)
    return FALSE;

  /* Parse the settings file. */
  while (fgets(fp, buffer, LIRC_SETTINGS_BUFFER_SIZE) != NULL)
  {
    switch (line)
    {
      case (int)LIRC_SETTINGS_LINE_PORT:
      {
        settings->port = atoi(buffer);
        break;
      }
      case (int)LIRC_SETTINGS_LINE_NAME:
      {
	strcpy(settings->server_name, buffer);
        break;
      }
      case (int)LIRC_SETTINGS_LINE_MOTD:
      {
	strcpy(settings->motd, buffer);
        break;
      }
      default:
      {
	fprintf(stderr, "Settings file has too many lines.\n");
	fclose(fp);
	exit(EXIT_FAILURE);
        break;
      }
    }

    line++;
  }
    
  fclose(fp);
  return TRUE;
}
