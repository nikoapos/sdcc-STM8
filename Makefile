# The STM8 model. Can be one of: 
# - STM8S103F2
# - STM8S103F3
# Other models which have the same register addresses and pin configuration as
# the above (at least for the used functionality) will also work, but have not
# been tested.
STM8_MODEL = STM8S103F2

CC = sdcc-sdcc
CFLAGS = --Werror --std-sdcc11 -mstm8 -D$(STM8_MODEL) -Iinclude
LDFLAGS = -lstm8 -mstm8 --out-fmt-ihx

HEADERS = $(wildcard *.h include/*.h)
EXAMPLE_IHX_FILES = $(patsubst src/examples/%.c, build/examples/%.ihx, $(wildcard src/examples/*.c))
PROGRAM_IHX_FILES = $(patsubst src/programs/%.c, build/programs/%.ihx, $(wildcard src/programs/*.c))

all: examples programs ;

examples: build $(EXAMPLE_IHX_FILES) ;

programs: build $(PROGRAM_IHX_FILES) ;

build/examples/%.rel: src/examples/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

build/programs/%.rel: src/programs/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXAMPLE_IHX_FILES): %.ihx: %.rel
	$(CC) $(LDFLAGS) $< -o $@

$(PROGRAM_IHX_FILES): %.ihx: %.rel
	$(CC) $(LDFLAGS) $< -o $@

build:
	mkdir build
	mkdir build/examples
	mkdir build/programs

clean:
	rm -rf build