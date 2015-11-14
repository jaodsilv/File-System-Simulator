#include "../headers/fildir.h"

/*Get the local time and write it in the following format: DD/MM/YYYY HH:MM:SS*/
void get_and_format_time(char *output)
{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime); timeinfo = localtime(&rawtime);
    sprintf(output, "%d/%d/%d %d:%d:%d",timeinfo->tm_mday, timeinfo->tm_mon + 1,
      timeinfo->tm_year + 1900, timeinfo->tm_hour,
      timeinfo->tm_min, timeinfo->tm_sec);
}
