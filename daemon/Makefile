INCLUDE_DIRS = .
INCLUDE_OPTS = $(patsubst %,-I%,$(INCLUDE_DIRS))
CC		= gcc


C_FILES		= network.c
OBJ_FILES	= $(patsubst %.c,dist/%.o,$(C_FILES))

all: clean build

dist/%.o: %.c
	$(CC) -c -o $@ $< $(INCLUDE_OPTS)

build: $(OBJ_FILES)
	$(CC) -o dist/main $^

clean:
	-rm dist/*
