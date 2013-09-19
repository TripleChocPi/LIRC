#include "lirc.h"
#include "settings.h"
#include "server.h"

void display_welcome(void);

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
    perror("LIRC could not read the settings file");
    return EXIT_FAILURE;
  }

  /* Initialize the server and bind to the port specified
   * in the settings file. */
  /*
  lirc_server_init(&lirc_server, &lirc_settings);
  if(!(lirc_server_bind(&lirc_server)))
  {
    perror("LIRC could not bind to the port specified");
    return EXIT_FAILURE;
  }
  */

  /* Enter the main loop of the server application */

  lirc_settings_destroy(&lirc_settings);
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
