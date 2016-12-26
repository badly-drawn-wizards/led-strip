#include <pulse/pulseaudio.h>
#include <pulse/simple.h>
#include <pulse/error.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef _PULSE_H_
#define _PULSE_H_

typedef struct {
  pa_simple *simpl;
  int16_t *buf;
  size_t buf_size;
} pulse_context;

pulse_context *pulse_context_new(size_t buf_size);

void pulse_context_free(pulse_context *ctxt);

void pulse_get_sample(pulse_context *ctxt);

#endif
