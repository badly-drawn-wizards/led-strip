#include <visualizers/visualizer.h>
#include <color.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#ifndef _VOLUME_VISUALIZER_H_
#define _VOLUME_VISUALIZER_H_

void volume_visualizer(double time, int16_t *samples, size_t num_samples, color *led_colors, size_t num_leds);

#endif
