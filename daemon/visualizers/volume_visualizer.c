#include <visualizers/volume_visualizer.h>

#define CLAMP(min,max,val) ((val < min) ? min : ((val > max) ? max : val))

void volume_visualizer(double time, int16_t *samples, size_t num_samples, color *led_color, size_t num_leds) {
  uint32_t total = 0;
  for(size_t i=0; i<num_samples; i++) {
    total += abs(samples[i]);
  }
  double volume = total / 200.0 / num_samples / num_leds;
  for(uint16_t i=0; i<num_leds; i++) {
    double pos = fabs(i * 1.0 / num_leds - 0.5);
    double val = pos < volume ? 1.0 : pow(CLAMP(0,1,1 - pos + volume), 20);
    from_hsv_3d(time/5+pos/3, 1.0, val, &led_color[i]);
  }
}
