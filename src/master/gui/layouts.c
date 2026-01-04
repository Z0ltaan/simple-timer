#include "gui/layouts.h"
#include <gtk/gtk.h>

GtkWidget *master__create_main_layout() {
  GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

  gtk_widget_set_halign(main_box, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(main_box, GTK_ALIGN_CENTER);

  return main_box;
}

GtkWidget *master__create_notebook_layout() {
  GtkWidget *notebook = gtk_notebook_new();

  return notebook;
}

GtkWidget *master__create_timer_layout() {
  GtkWidget *timer_creation_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_widget_set_halign(timer_creation_box, GTK_ALIGN_CENTER);
  return timer_creation_box;
}

GtkWidget *master__create_timer_list_layout() {
  GtkWidget *timer_list_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  return timer_list_box;
}
