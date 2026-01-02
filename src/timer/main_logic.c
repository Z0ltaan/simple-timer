#include "main_logic.h"
#include "audio/data_callback.h"
#include "constants.h"
#include "gui/layouts.h"
#include "miniaudio/miniaudio.h"
#include "timestamp_interaction.h"
#include <bits/types/struct_timespec.h>
#include <glib.h>
#include <glibconfig.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main_logic(int argc, char **argv) {
  time_t previous_timestamp = 7;
  time_t interval = 100;
  bool stopped = false;
  bool should_close = false;

  ma_result result;
  ma_decoder decoder;
  ma_device_config deviceConfig;
  ma_device device;

  result = ma_decoder_init_file("./build/pickupCoin.wav", NULL, &decoder);
  if (result != MA_SUCCESS) {
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
// int main(int argc, char **argv) {
//   char program_path[256];
//   memcpy(program_path, argv[0], (strrchr(argv[0], '/') - argv[0]));
//   printf("%s\n", program_path);
//   ma_result result;
//   ma_engine engine;
//   result = ma_engine_init(NULL, &engine);
//   if (result != MA_SUCCESS) {
//     return -1;
//   }
//   for (int i = 0; i < 10; ++i) {
//     if (ma_engine_play_sound(&engine, "./build/synth.wav", NULL) !=
//         MA_SUCCESS) {
//       printf("shit\n");
//     }
//   }
//   getchar();
//   ma_engine_uninit(&engine);
//   return 0;
// }
