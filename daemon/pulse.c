#include <pulse/pulseaudio.h>
#include <pulse/simple.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char *default_sink_monitor;
  pa_mainloop *mainloop;
} cb_data;

void server_info_cb(pa_context *context, const pa_server_info *server_info, void *data) {
  cb_data *d = data;
  strcat(strcpy(d->default_sink_monitor,server_info->default_sink_name),".monitor");
  pa_mainloop_quit(d->mainloop, 0);
}
void notify_cb(pa_context *context, void *data) {
  switch(pa_context_get_state(context)) {
  case PA_CONTEXT_READY:
    pa_operation_unref(pa_context_get_server_info(context, server_info_cb, data));
    break;
  default:
    break;
  }
}

char* get_default_sink_monitor() {
  pa_mainloop *mainloop = pa_mainloop_new();
  pa_mainloop_api *mainloop_api = pa_mainloop_get_api(mainloop);
  pa_context *context = pa_context_new(mainloop_api, "led-strip-daemon-default-sink-getter");

  char* default_sink_monitor = malloc(sizeof(char)*1024);
  cb_data userdata = {.default_sink_monitor = default_sink_monitor, .mainloop = mainloop};

  pa_context_connect(context, NULL, PA_CONTEXT_NOFLAGS, NULL);
  pa_context_set_state_callback(context, notify_cb, &userdata);

  int ret;
  if(pa_mainloop_run(mainloop, &ret) < 0) {
    printf("Could not open pulseaudio mainloop.\n");
  };

  pa_context_disconnect(context);
  pa_mainloop_free(mainloop);

  return default_sink_monitor;
}

void get_sample(int16_t *buf, size_t buf_len, pa_simple* simpl) {
  int err;
  if(pa_simple_read(simpl, buf, buf_len, &err) < 0) {
    printf("pa_simple_read failed");
    exit(EXIT_FAILURE);
  }
}

#define BUF_SIZE 2048


int main() {
  char* default_sink_monitor = get_default_sink_monitor();
  printf("%s\n", default_sink_monitor);

  pa_sample_spec sample_spec = {
    .format = PA_SAMPLE_S16LE,
    .rate = 44100,
    .channels = 1
  };

  pa_buffer_attr buffer_attr = {
    .maxlength = BUF_SIZE*2,
    .fragsize = BUF_SIZE
  };


  int16_t buf[BUF_SIZE];
  int error;


  pa_simple *simpl = pa_simple_new(NULL, "led-strip-daemon", PA_STREAM_RECORD, default_sink_monitor, "led-strip-stream", &sample_spec, NULL, &buffer_attr, &error);


  if(simpl == NULL) {
    printf("failed to open source\n");
    exit(EXIT_FAILURE);
  }

  while(1) {
    get_sample(buf, BUF_SIZE, simpl);
    long total = 0;
    for(int i=0; i<BUF_SIZE; i++) {
      total+=abs(buf[i]);
      /* printf("%d\n", buf[i]); */
      /* printf("\33[A\33[2K\r"); */
    }
    total /= BUF_SIZE;

    for(int i=0; i<total/100; i++) {
      printf("#");
    }
    printf("\n");

    /* printf("\33[A\33[2K\r"); */
  }


  pa_simple_free(simpl);
}
