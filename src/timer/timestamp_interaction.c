#include "timestamp_interaction.h"
#include "constants.h"
#include <stdio.h>

bool timer__update_timestamp(time_t *to_update, size_t ticks,
                             size_t ticks_per_second) {
  if (!(ticks % ticks_per_second)) {
    *to_update -= ticks / ticks_per_second;
    return true;
  }
  return false;
}

// NOTE: should be time[0] == '0';
char *timer__get_formated_time(char *time, int digits) {
  return time[digits] == '\0' ? time : time + 1;
}

void timer__place_formatted_time_string(char *to_place, time_t time) {
  char hours[4];
  char minutes[4];
  char seconds[4];

  sprintf(hours, "0%ld", time / 3600);
  sprintf(minutes, "0%ld", (time % 3600) / 60);
  sprintf(seconds, "0%ld", time % 60);

  sprintf(to_place, "%s : %s : %s",
          timer__get_formated_time(hours, TIME_DIGITS),
          timer__get_formated_time(minutes, TIME_DIGITS),
          timer__get_formated_time(seconds, TIME_DIGITS));
}
