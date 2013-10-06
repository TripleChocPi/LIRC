#include "settings.h"

/* LIRC Settings file functions */
BOOL
lirc_init_settings(LIRCSettings* settings, const char* file_name)
{
  FILE* fp = NULL;
  LIRC_SETTINGS_LINE line = 0;
  char buffer[LIRC_SETTINGS_BUFFER_SIZE];

  /* Init settings */
  settings->port = 0;
  settings->server_name = NULL;
  settings->motd = NULL;

  fp = fopen(file_name, "r");

  /* There was an error opening the settings file. */
  if (fp == NULL)
  {
    perror("LIRC could not read the settings file");
    return FALSE;
  }

  /* Parse the settings file. */
  while (fgets(buffer, LIRC_SETTINGS_BUFFER_SIZE, fp) != NULL)
  {
    switch (line)
    {
    case (int)LIRC_SETTINGS_LINE_PORT:
    {
      errno = 0;
      settings->port = strtol(buffer, NULL, 10);

      if (errno != 0)
      {
        perror("An error occured parsing the port number");
        fclose(fp);
        return FALSE;
      }
      break;
    }
    case (int)LIRC_SETTINGS_LINE_NAME:
    {
      settings->server_name = safe_strdup(buffer);
      break;
    }
    case (int)LIRC_SETTINGS_LINE_MOTD:
    {
      settings->motd = safe_strdup(buffer);
      break;
    }
    default:
    {
      perror("Settings file has too many lines");
      fclose(fp);
      return FALSE;
    }
    }

    line++;
  }

  fclose(fp);
  return TRUE;
}

void
lirc_settings_destroy(LIRCSettings* settings)
{
  free(settings->server_name);
  free(settings->motd);
}
