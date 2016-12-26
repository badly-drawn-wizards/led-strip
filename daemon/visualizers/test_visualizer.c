#include <visualizers/volume_visualizer.h>

#define CLAMP(min,max,val) ((val < min) ? min : ((val > max) ? max : val))

void test_visualizer(double time, int16_t *samples, size_t num_samples, color *led_color, size_t num_leds) {
  uint16_t pos = fmod(time/10.0,1.0)*num_leds;
  for(uint16_t i=0; i<num_leds; i++) {
    from_rgb_3i(255, 0, 0, &led_color[i]);
  }
}
