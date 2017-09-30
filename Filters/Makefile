CLASSES=modulator transposer resampler fshifter multiecho echo envelope \
	reverse loop amplifier offset filter

LIBHDRS=${CLASSES:S/$/.h/g}
LIBSRCS=${CLASSES:S/$/.cc/g}
LIBOBJS=${CLASSES:S/$/.o/g}

LIB=musflt

CHECKINS=$(LIBSRCS) $(LIBHDRS) Makefile

CCPP=g++
CFLAGS=-c -g -Wall -I../Sources

all: lib

lib: lib$(LIB).a

lib$(LIB).a: $(LIBOBJS)
	ar -r lib$(LIB).a $(LIBOBJS)
	ranlib lib$(LIB).a

depend:
	mkdep $(CFLAGS) $(LIBSRCS)

checkin:
	-ci -l $(CHECKINS)

clean:
	-rm $(LIBOBJS) lib$(LIB).a

.include <bsd.dep.mk>

