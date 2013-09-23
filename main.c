#include "lirc.h"
#include "settings.h"
#include "server.h"

void display_welcome(void);
void setup_server(LIRCSettings*, LIRCServer*);

int
main(int argc, char *argv[])
{
  LIRCSettings lirc_settings;
  LIRCServer lirc_server;

  /* Display a welcome message to the console. */
  display_welcome();

  /* Set up the LIRC server */
  setup_server(&lirc_settings, &lirc_server);

  /* Enter the main loop of the server application */
  sleep(20);

  printf ("Server is shutting down.\n");
  lirc_server_close(&lirc_server);
  lirc_settings_destroy(&lirc_settings);
  return EXIT_SUCCESS;
}

void
display_welcome()
{
  printf("LIRC Server v%s starting...\n", LIRC_VERSION);
  printf("Developed by TripleChocPi 2013.\n\n");
  printf("LIRC is an open-source project released under the GPL v3.\n");
  printf("Pull requests are welcome. For more information on LIRC's\n");
  printf("design philosophy and goals please visit our GitHub repo\n");
  printf("at %s\n\n", LIRC_GITHUB_ADDRESS);
}

void
setup_server(LIRCSettings* lirc_settings, LIRCServer* lirc_server)
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
  if(!(lirc_server_init(lirc_server, lirc_settings)))
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

  printf("Server socket bound and listening on port %d.\n",
         lirc_settings->port);
}
