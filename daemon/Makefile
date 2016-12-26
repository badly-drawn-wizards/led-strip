INCLUDE_DIRS = .
INCLUDE_OPTS = $(patsubst %,-I%,$(INCLUDE_DIRS))
LIBS = pulse pulse-simple SDL2 m
LIB_OPTS = $(patsubst %,-l%,$(LIBS))
CC		= gcc


C_FILES		= main.c pulse.c network.c color.c backends/sdl_backend.c backends/led_strip_backend.c visualizers/volume_visualizer.c visualizers/test_visualizer.c
OBJ_FILES	= $(patsubst %.c,dist/%.o,$(C_FILES))

all: clean build

dist/%.o: %.c
	$(CC) -g -c -o $@ $< $(INCLUDE_OPTS)

build: $(OBJ_FILES)
	$(CC) -g -o dist/main $^ $(LIB_OPTS)

addcap:
	setcap cap_net_raw+pei dist/main

clean:
	-rm dist/*
