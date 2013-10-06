#include "lirc.h"
#include "settings.h"
#include "server.h"

void display_welcome(void);
void setup_server(LIRCSettings*, LIRCServer*);

int
main(int argc, char *argv[])
{
  LIRCServer lirc_server;
  LIRCSettings lirc_settings;
  BOOL handle_messages = TRUE;

  /* Display a welcome message to the console. */
  display_welcome();

  /* Set up the LIRC server */
  lirc_server_init(&lirc_server, &lirc_settings);

  /* Enter the main loop of the server application */
  while (TRUE)
  {
    lirc_server_main_loop(&lirc_server, &lirc_settings);
  }

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
