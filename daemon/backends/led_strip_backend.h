#include <backends/backend.h>
#include <color.h>
#include <network.h>

#include <stdint.h>

#ifndef _LED_STRIP_BACKEND_H_
#define _LED_STRIP_BACKEND_H_

void *led_strip_backend_init();
void led_strip_send_leds(color *led_colors, size_t num_leds, void* backend);
void led_strip_backend_free(void* ptr);
void get_led_strip_backend(led_backend *be);
#endif
