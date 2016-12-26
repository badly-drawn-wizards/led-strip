#include<backends/backend.h>

#include <color.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#ifndef _SDL_BACKEND_H_
#define _SDL_BACKEND_H_

void *sdl_backend_init();
void sdl_send_leds(color *led_colors, size_t num_leds, void* backend);
void sdl_backend_free(void* ptr);

void get_sdl_backend(led_backend *be);

#endif
