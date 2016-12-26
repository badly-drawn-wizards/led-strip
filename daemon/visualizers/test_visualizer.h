#include <visualizers/visualizer.h>
#include <color.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#ifndef _TEST_VISUALIZER_H_
#define _TEST_VISUALIZER_H_

void test_visualizer(double time, int16_t *samples, size_t num_samples, color *led_colors, size_t num_leds);

#endif
