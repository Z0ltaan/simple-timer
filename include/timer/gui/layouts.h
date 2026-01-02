#ifndef LAYOUTS_H
#define LAYOUTS_H

#include <gtk/gtk.h>

struct TimerButtonData
{
  bool* stopped;
  time_t* previous_timestamp;
};
typedef struct TimerButtonData TimerButtonData;

GtkWidget*
timer__create_timestamp_layout(void* data);

GtkWidget*
timer__create_buttons_layout(void* data);

#endif
