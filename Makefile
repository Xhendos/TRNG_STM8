CC=sdcc
CFLAGS=--std-sdcc99
CFLAGS+=-mstm8
CFLAGS+=-lstm8
CFLAGS+=--out-fmt-ihx

OBJS=*.map
OBJS+=*.rel
OBJS+=*.rst
OBJS+=*.lst
OBJS+=*.sym
OBJS+=*.asm
OBJS+=*.cdb
OBJS+=*.ihx
OBJS+=*.lk

all: src/main.c
	$(CC) $(CFLAGS) src/main.c

.PHONY: clean
clean:
	rm -f $(OBJS)
