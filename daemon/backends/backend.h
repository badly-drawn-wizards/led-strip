#include<color.h>
#include<stdlib.h>
#include<stdint.h>
#include<math.h>

#ifndef _BACKEND_H_
#define _BACKEND_H_


typedef struct {
  void* (*backend_init)();
  void (*send_leds)(color *led_colors, size_t num_leds, void* backend);
  void (*backend_free)(void* ptr);
} led_backend;

#endif
