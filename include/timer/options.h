#ifndef OPTIONS_H
#define OPTIONS_H

#define VAL_SHORT_HELP_OPT 'h'
#define VAL_SHORT_PATH_OPT 's'
#define VAL_SHORT_TIME_OPT 't'
#define RETURN_VAL NULL

#include <time.h>

void
timer__set_time(time_t* to_set, char* value);

void
timer__set_sound_file_path(char* path_to_set, const char* src, int char_count);

void
timer__print_help();

#endif
