CHIP	= 16F1825
INCLUDE_DIRS = .

INCLUDE_OPTS = $(patsubst %,-I%,$(INCLUDE_DIRS))
LINKER_OPTS = -L-pbigram=BIGRAM

CC		= xc8 --chip=$(CHIP) $(INCLUDE_OPTS)


C_FILES		= main.c
P1_FILES	= $(patsubst %.c,dist/%.p1,$(C_FILES))

test:
	echo $(CC)

all: clean build upload

dist/%.p1: %.c
	$(CC) --pass1 -O$@ $<

build: $(P1_FILES)
	$(CC) -M -Odist/main $(LINKER_OPTS) $^

upload:
	mdb mdb.txt

clean:
	-rm dist/*
