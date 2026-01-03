#include "main_logic.h"
#include "audio/data_callback.h"
#include "constants.h"
#include "gui/layouts.h"
#include "miniaudio/miniaudio.h"
#include "options.h"
#include "timestamp_interaction.h"
#include <getopt.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main_logic(int argc, char **argv) {
  time_t previous_timestamp = 0;
  char sound_path[256];

  time_t interval = 100;

  // NOTE: handle cl args
  const char *short_options = "hs:t:";
  const struct option long_options[] = {
      {"help", no_argument, RETURN_VAL, VAL_SHORT_HELP_OPT},
      {"sound-path", required_argument, RETURN_VAL, VAL_SHORT_PATH_OPT},
      {"time", required_argument, RETURN_VAL, VAL_SHORT_TIME_OPT},
      {NULL, 0, NULL, 0}};

  bool time_is_set = false;
  bool sound_is_set = false;

  char rez = 0;
  while ((rez = getopt_long(argc, argv, short_options, long_options, NULL)) !=
         -1) {
    switch (rez) {
    case VAL_SHORT_HELP_OPT:
      timer__print_help();
      exit(0);
      break;
    case VAL_SHORT_PATH_OPT:
      timer__set_sound_file_path(sound_path, optarg, strlen(optarg));
      sound_is_set = true;
      break;
    case VAL_SHORT_TIME_OPT:
      timer__set_time(&previous_timestamp, optarg);
      time_is_set = true;
      break;
    case '?':
      exit(-1);
    default:
      break;
    }
  }

  if (!(time_is_set && sound_is_set)) {
    perror("Usage: simple-timer <time-opt> <sound-path-opt>\n");
    return -1;
  }

  // NOTE: init miniaudio
  ma_result result;
  ma_decoder decoder;
  ma_device_config deviceConfig;
  ma_device device;

  if (ma_decoder_init_file(sound_path, NULL, &decoder) != MA_SUCCESS) {
    printf("Could not initialize sound file w path: %s", sound_path);
    return -2;
  }

  ma_data_source_set_looping(&decoder, MA_TRUE);

  deviceConfig = ma_device_config_init(ma_device_type_playback);
  deviceConfig.playback.format = decoder.outputFormat;
  deviceConfig.playback.channels = decoder.outputChannels;
  deviceConfig.sampleRate = decoder.outputSampleRate;
  deviceConfig.dataCallback = data_callback;
  deviceConfig.pUserData = &decoder;

  if ((ma_device_init(NULL, &deviceConfig, &device)) != MA_SUCCESS) {
    printf("Failed to init or open playback device.\n");
    ma_decoder_uninit(&decoder);
    ma_device_uninit(&device);
    return -3;
  }

  bool stopped = false;
  bool should_close = false;

  // NOTE: init gtk
  gtk_init();
  GtkWidget *window = gtk_window_new();
  GtkWidget *timestamp_label;
  gtk_window_set_title(GTK_WINDOW(window), "SimpleTimer");

  {
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    TimerButtonData dt = {&stopped, &previous_timestamp};
    GtkWidget *timestamp_layout = timer__create_timestamp_layout(NULL);

    // TODO: write proper find child function
    timestamp_label = gtk_widget_get_first_child(timestamp_layout);
    gtk_box_append(GTK_BOX(main_box), timestamp_layout);
    gtk_box_append(GTK_BOX(main_box), timer__create_buttons_layout(&dt));

    gtk_window_set_child(GTK_WINDOW(window), main_box);
  }

  gtk_window_present(GTK_WINDOW(window));

  char final_time_string[12];
  struct timespec requested = {0l, interval * MILISECONDS};
  size_t inner_ticks = 0;
  bool start_beeping = false;

  // NOTE: main loop
  while (g_list_model_get_n_items(gtk_window_get_toplevels()) > 0) {
    g_main_context_iteration(NULL, false);

    if (!stopped) {
      inner_ticks += 1;
      if (timer__update_timestamp(&previous_timestamp, inner_ticks, interval)) {

        if (!previous_timestamp) {
          start_beeping = true;
          stopped = true;
          GtkWidget *w =
              gtk_widget_get_last_child(gtk_widget_get_first_child(window));
          gtk_widget_set_visible(w, FALSE);
        }
        inner_ticks = 0;
      }
    } else if (start_beeping) {
      ma_device_start(&device);
      start_beeping = false;
    }

    if (GTK_IS_WIDGET(timestamp_label)) {
      timer__place_formatted_time_string(final_time_string, previous_timestamp);
      gtk_label_set_label(GTK_LABEL(timestamp_label), final_time_string);
    }

    if (nanosleep(&requested, NULL) < 0) {
      break;
      perror("fucking nanosleep");
    }
  }

  ma_device_uninit(&device);
  ma_decoder_uninit(&decoder);

  return 0;
}
