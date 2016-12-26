#include <time.h>
#include <pulse.h>
#include <backends/sdl_backend.h>
#include <backends/led_strip_backend.h>
#include <visualizers/volume_visualizer.h>
#include <visualizers/test_visualizer.h>

#define BUF_SIZE 512
#define NUM_LEDS 120
#define PERIOD 100000

int main() {
  led_backend led_strip_backend;
  led_backend sdl_backend;
  get_led_strip_backend(&led_strip_backend);
  get_sdl_backend(&sdl_backend);
  led_visualizer visualizer = volume_visualizer;

  color led_colors[NUM_LEDS];
  pulse_context *pulse = pulse_context_new(BUF_SIZE);
  void *led_strip_backend_context = led_strip_backend.backend_init();
  void *sdl_backend_context = sdl_backend.backend_init();
  uint16_t cnt = 0;
  struct timeval now;
  while(1) {
    gettimeofday(&now, NULL);
    suseconds_t usec = now.tv_usec;
    double time = now.tv_sec + now.tv_usec / 1E6;
    pulse_get_sample(pulse);
    visualizer(time, pulse->buf, pulse->buf_size, led_colors, NUM_LEDS);
    led_strip_backend.send_leds(led_colors, NUM_LEDS, led_strip_backend_context);
    sdl_backend.send_leds(led_colors, NUM_LEDS, sdl_backend_context);

    gettimeofday(&now, NULL);
    uint32_t elapsed = now.tv_usec - usec;
    usleep(elapsed<PERIOD ? PERIOD-elapsed : 0);
    ++cnt;
  }
  led_strip_backend.backend_free(led_strip_backend_context);
  sdl_backend.backend_free(sdl_backend_context);
  pulse_context_free(pulse);
}
