#ifndef TIMESTAMP_INTERACTION_H
#define TIMESTAMP_INTERACTION_H

#include <stdbool.h>
#include <time.h>

char*
timer__get_formated_time(char* time, int digits);

bool
timer__update_timestamp(time_t* to_update,
                        size_t ticks,
                        size_t ticks_per_second);

void
timer__place_formatted_time_string(char* to_place, time_t time);

#endif
