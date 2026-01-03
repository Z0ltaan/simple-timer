#include "options.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void timer__set_time(time_t *to_set, char *value) {
  time_t time = atol(value);
  *to_set = time;
}

void timer__set_sound_file_path(char *path_to_set, const char *src,
                                int char_count) {
  memcpy(path_to_set, src, char_count);
}

void timer__print_help() {
  printf("simple-timer <timer-opt> <sound-path-opt>\n");
  printf("-h, --help        Print help\n");
  printf("-t, --time        Set count down time\n");
  printf("-s, --sound-path  Set timer sound path\n");
}
