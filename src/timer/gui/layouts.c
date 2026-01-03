#include "gui/layouts.h"
#include "constants.h"

static void timer__stop_button_clicked(GtkButton *button, gpointer data) {
  bool *status = (bool *)data;
  *status = !(*status);
}

static void timer__add_minute_button_clicked(GtkButton *button, gpointer data) {
  time_t *add_to = (time_t *)data;
  if (UPPER_TIME_BOUNDARY - *add_to < 60) {
    *add_to += UPPER_TIME_BOUNDARY - *add_to;
  } else {
    *add_to += 60;
  }
}

GtkWidget *timer__create_timestamp_layout(void *data) {
  GtkWidget *timestamp_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  GtkWidget *timestamp_label = gtk_label_new("timestamp");

  gtk_widget_set_halign(timestamp_box, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(timestamp_box, GTK_ALIGN_CENTER);

  gtk_box_append(GTK_BOX(timestamp_box), timestamp_label);

  return timestamp_box;
}

GtkWidget *timer__create_buttons_layout(void *data) {
  if (data == NULL) {
    return data;
  }

  TimerButtonData *dt = (TimerButtonData *)data;
  GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  GtkWidget *stop_button = gtk_button_new_with_label("Stop");
  g_signal_connect(stop_button, "clicked",
                   G_CALLBACK(timer__stop_button_clicked), dt->stopped);
  GtkWidget *add_minute_button = gtk_button_new_with_label("Add 1 minute");
  g_signal_connect(add_minute_button, "clicked",
                   G_CALLBACK(timer__add_minute_button_clicked),
                   dt->previous_timestamp);

  gtk_widget_set_halign(button_box, GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(button_box), stop_button);
  gtk_box_append(GTK_BOX(button_box), add_minute_button);

  return button_box;
}
