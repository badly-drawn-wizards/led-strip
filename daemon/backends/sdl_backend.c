#include <backends/sdl_backend.h>

void* sdl_get_or_die(void* ptr) {
  if(ptr == NULL) {
    printf("%s\n", SDL_GetError());
    exit(1);
  }
  return ptr;
}
void sdl_try_or_die(int res) {
  if(res != 0) {
    printf("%s\n", SDL_GetError());
    exit(1);
  }
}

void handle_events() {
  SDL_Event e;
  SDL_PollEvent(&e);
  switch(e.type) {
  case SDL_QUIT:
    exit(0);
    break;
  }
}

void draw(SDL_Renderer *renderer, color *led_colors, size_t leds_count) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_Rect rect;
  rect.h = 6;
  rect.w = 6;
  rect.y = 44;
  for(size_t i=0; i<leds_count; ++i) {
    SDL_SetRenderDrawColor(renderer, led_colors[i].r, led_colors[i].g, led_colors[i].b, 0xFF);
    rect.x = 10*i + 2;
    SDL_RenderFillRect(renderer, &rect);
  }
}

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
} sdl_backend_data;

void* sdl_backend_init() {
  sdl_backend_data *data = malloc(sizeof(sdl_backend_data));
  sdl_try_or_die(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER));
  atexit(SDL_Quit);
  data->window=sdl_get_or_die(SDL_CreateWindow("Led Preview", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1200, 100, SDL_WINDOW_SHOWN));
  data->renderer=SDL_CreateRenderer(data->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  return data;
}

void sdl_send_leds(color *led_colors, size_t num_leds, void* d) {
  sdl_backend_data *data = d;
  handle_events();
  draw(data->renderer, led_colors, num_leds);
  SDL_RenderPresent(data->renderer);
}

void sdl_backend_free(void *d) {
  free(d);
  SDL_Quit();
}

void get_sdl_backend(led_backend *be) {
  be->backend_init = sdl_backend_init;
  be->send_leds = sdl_send_leds;
  be->backend_free = sdl_backend_free;
}
