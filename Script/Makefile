
CHECKINS=tclMusic.c Makefile
OBJS=tclMusic.o
CFLAGS=-g -I../Sources -I../Filters -I../Drivers
LFLAGS=-g -L ../Drivers -lmusdrv -L../Filters -lmusflt -L../Sources -lmussrc \
	-L ../Filters -lmusflt -ltcl -lm

all : tclmus

install: /usr/local/bin/tclmus

/usr/local/bin/tclmus: tclmus
	cp tclmus /usr/local/bin

tclmus: $(OBJS) ../Drivers/libmusdrv.a ../Filters/libmusflt.a \
		../Sources/libmussrc.a
	c++ -o tclmus $(OBJS) $(LFLAGS)

tclMusic.o: tclMusic.c
	c++ $(CFLAGS) -c tclMusic.c

depend:
	mkdep $(CFLAGS) tclMusic.c

checkin:
	-ci -l $(CHECKINS)

clean:
	-rm $(OBJS) tclmus

.include <bsd.dep.mk>

