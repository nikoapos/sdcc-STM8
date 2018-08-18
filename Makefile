# The STM8 model. Can be one of:
#   - STM8S103 : For STM8S103F2, STM8103F3 and STM8S103K3 chips
STM8_MODEL = STM8S103

CC = sdcc-sdcc
CFLAGS = --Werror --std-sdcc11 -mstm8 -D$(STM8_MODEL) -Iinclude
LDFLAGS = -lstm8 -mstm8 --out-fmt-ihx

HEADERS = $(wildcard *.h include/*.h)
IHX_FILES = $(patsubst src/%.c, build/%.ihx, $(wildcard src/*.c))

all: build $(IHX_FILES)

build/%.rel: src/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(IHX_FILES): %.ihx: %.rel
	$(CC) $(LDFLAGS) $< -o $@

build:
	mkdir build

clean:
	rm build/*
