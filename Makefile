CC=sdcc
CFLAGS=--std-sdcc99
CFLAGS+=-mstm8
CFLAGS+=-lstm8
CFLAGS+=--debug

OBJS=*.map
OBJS+=*.rel
OBJS+=*.rst
OBJS+=*.lst
OBJS+=*.sym
OBJS+=*.asm
OBJS+=*.cdb
OBJS+=*.ihx
OBJS+=*.lk
OBJS+=*.adb
OBJS+=*.elf

all: src/main.c
	$(CC) $(CFLAGS) --out-fmt-ihx --nogcse --all-callee-saves --verbose --stack-auto --fverbose-asm --float-reent --no-peep src/main.c

debug: src/main.c
	$(CC) $(CFLAGS) --out-fmt-elf --all-callee-saves --verbose --stack-auto --fverbose-asm --float-reent --no-peep src/main.c

.PHONY: clean
clean:
	rm -f $(OBJS)
