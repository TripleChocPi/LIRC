#include "settings.h"

/* LIRC Settings file functions */
BOOL
lirc_init_settings(LIRCSettings* settings, const char* file_name)
{
  FILE* fp = NULL;
  LIRC_SETTINGS_LINE line = 0;
  char buffer[LIRC_SETTINGS_BUFFER_SIZE];

  fp = fopen(file_name, "r");

  /* There was an error opening the settings file. */
  if (fp == NULL)
    return FALSE;

  /* Parse the settings file. */
  while (fgets(buffer, LIRC_SETTINGS_BUFFER_SIZE, fp) != NULL)
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
      exit(EXIT_FAILURE);
      break;
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
