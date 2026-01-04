#include "main_logic.h"
#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_TIMER_COUNT 8

void master__report_and_exit(const char *err);

int main_logic(int argc, char **argv) {
  long active_timers = 0;

  // NOTE: create main layout
  gtk_init();
  GtkWidget *window = gtk_window_new();
  gtk_window_set_title(GTK_WINDOW(window), "MasterTimer");
  {

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    gtk_widget_set_halign(main_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(main_box, GTK_ALIGN_CENTER);

    gtk_window_set_child(GTK_WINDOW(window), main_box);
  }

  gtk_window_present(GTK_WINDOW(window));

  while (g_list_model_get_n_items(gtk_window_get_toplevels()) > 0) {
    g_main_context_iteration(NULL, true);
  }

  return 0;
}

void master__report_and_exit(const char *err) {
  perror(err);
  exit(-1);
}
