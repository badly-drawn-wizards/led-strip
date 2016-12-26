#include <backends/led_strip_backend.h>

typedef struct {
  network_context *network;
} led_strip_backend_data;

char IF_NAME[] = "enp4s0";
char MAC_ADDR[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};


void *led_strip_backend_init() {
  led_strip_backend_data *data = malloc(sizeof(led_strip_backend_data));
  data->network = network_context_new(IF_NAME, MAC_ADDR);
  return data;
}
void led_strip_send_leds(color *led_colors, size_t num_leds, void* d) {
  led_strip_backend_data *data = d;
  send_raw_frame(data->network, led_colors, num_leds*sizeof(color));
}
void led_strip_backend_free(void* d) {
  led_strip_backend_data *data = d;
  network_context_free(data->network);
  free(data);
}

void get_led_strip_backend(led_backend *be) {
  be->backend_init = led_strip_backend_init;
  be->send_leds = led_strip_send_leds;
  be->backend_free = led_strip_backend_free;
};
