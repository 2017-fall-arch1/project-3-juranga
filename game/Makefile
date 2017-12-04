COMMON_OBJECTS = 
CPU = msp430g2553
CFLAGS = -mmcu=${CPU} -Os -I../h
LDFLAGS = -L../lib -L/opt/ti/msp430_gcc/include

all: libCircle.a pong.elf

CC 	= msp430-elf-gcc
AS	= msp430-elf-as
AR 	= msp430-elf-ar

abCircle_decls.h abCircle.h chordVec.h libCircle.a: makeCircles.c abCircle.o _abCircle.h Makefile
	cc -o makeCircles makeCircles.c
	rm -rf circles; mkdir circles
	./makeCircles
	cat _abCircle.h abCircle_decls.h > abCircle.h
	(cd circles; $(CC) -I.. -I../../h -mmcu=${CPU} -Os -c *.c)
	$(AR) crs libCircle.a circles/*.o abCircle.o

abCircle.o: _abCircle.h abCircle.c

install: libCircle.a abCircle.h chordVec.h
	mkdir -p ../h ../lib
	cp libCircle.a ../lib
	cp abCircle.h chordVec.h ../h

pong.elf: ${COMMON_OBJECTS} wdInterruptHandler.o buzzer.o p1_interrupt_handler.o states.o switches.o game.o ../lib/libTimer.a libCircle.a
		${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lLcd -lTimer -lShape -o $@

load: pong.elf
	mspdebug rf2500 "prog pong.elf"

clean:
	rm -f libCircle.a abCircle.h abCircle_decls.h chordVec.h *.o *.elf makeCircles
	rm -rf circles
