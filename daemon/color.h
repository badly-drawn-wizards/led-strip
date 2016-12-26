#include <stdint.h>
#include <math.h>

#ifndef _COLOR_H_
#define _COLOR_H_

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} color;

void from_rgb_3i(uint8_t r, uint8_t g, uint8_t b, color *c);
void from_rgb_3d(double r, double g, double b, color *c);
void from_hsv_3d(double h, double s, double v, color *c);

#endif
