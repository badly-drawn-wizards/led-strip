#include <pulse.h>

typedef struct {
  char *default_sink_monitor;
  pa_mainloop *mainloop;
} default_sink_monitor_cb_data;

void default_sink_monitor_server_info_cb(pa_context *context, const pa_server_info *server_info, void *data) {
  default_sink_monitor_cb_data *d = data;
  strcat(strcpy(d->default_sink_monitor,server_info->default_sink_name),".monitor");
  pa_mainloop_quit(d->mainloop, 0);
}
void notify_cb(pa_context *context, void *data) {
  switch(pa_context_get_state(context)) {
  case PA_CONTEXT_READY:
    pa_operation_unref(pa_context_get_server_info(context, default_sink_monitor_server_info_cb, data));
    break;
  default:
    break;
  }
}

char PULSE_CLIENT_NAME[] = "led-strip-daemon";

char* get_default_sink_monitor() {
  pa_mainloop *mainloop = pa_mainloop_new();
  pa_mainloop_api *mainloop_api = pa_mainloop_get_api(mainloop);
  pa_context *context = pa_context_new(mainloop_api, PULSE_CLIENT_NAME);

  char* default_sink_monitor = malloc(sizeof(char)*1024);
  default_sink_monitor_cb_data userdata = {.default_sink_monitor = default_sink_monitor, .mainloop = mainloop};

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

pulse_context *pulse_context_new(size_t buf_size) {
  pulse_context *ctxt = malloc(sizeof(pulse_context));

  ctxt->buf = malloc(sizeof(ctxt->buf)*buf_size);
  ctxt->buf_size = buf_size;

  char* default_sink_monitor = get_default_sink_monitor();
  printf("Using sink '%s'\n", default_sink_monitor);
  pa_sample_spec sample_spec = {
    .format = PA_SAMPLE_S16LE,
    .rate = 44100,
    .channels = 1
  };
  pa_buffer_attr buffer_attr = {
    .maxlength = buf_size,
    .fragsize = buf_size
  };

  int error;
  pa_simple *simpl = pa_simple_new(NULL, PULSE_CLIENT_NAME, PA_STREAM_RECORD, default_sink_monitor, PULSE_CLIENT_NAME, &sample_spec, NULL, &buffer_attr, &error);
  if(simpl == NULL) {
    printf("%s\n", pa_strerror(error));
    exit(1);
  }
  ctxt->simpl = simpl;

  return ctxt;
}

void pulse_context_free(pulse_context *ctxt) {
  pa_simple_free(ctxt->simpl);
  free(ctxt->buf);
  free(ctxt);
}

void pulse_get_sample(pulse_context *ctxt) {
  int err;
  if(pa_simple_read(ctxt->simpl, ctxt->buf, ctxt->buf_size, &err) < 0) {
    printf("%s\n", pa_strerror(err));
    exit(1);
  }
}

