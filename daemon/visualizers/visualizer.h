#include <color.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef _VISUALIZER_H_
#define _VISUALIZER_H_

typedef void (*led_visualizer)(double time, int16_t *samples, size_t num_samples, color *led_colors, size_t num_leds);

#endif
