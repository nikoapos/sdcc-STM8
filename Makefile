CC = sdcc-sdcc
CFLAGS = --Werror --std-sdcc11 -mstm8 -DSTM8S103 -Iinclude
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
