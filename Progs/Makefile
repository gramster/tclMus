PROGS=playsam

PROGSRCS=${PROGS:S/$/.cc/g}
PROGOBJS=${PROGS:S/$/.o/g}

CHECKINS=$(PROGSRCS) Makefile

CCPP=g++
CFLAGS=-c -g -Wall -I../Drivers -I../Sources -I../Filters
LFLAGS=-L../Filters -lmusflt -L../Sources -lmussrc -L../Drivers -lmusdrv \
	-L../Filters -lmusflt

all: progs

progs: $(PROGOBJS) $(PROGS)

playsam: playsam.o
	$(CCPP) -o playsam playsam.o $(LFLAGS)

depend:
	mkdep $(CFLAGS) $(PROGSRCS)

checkin:
	-ci -l $(CHECKINS)

clean:
	-rm $(PROGOBJS) $(PROGS)

test:
	(cd ../samples; ../Progs/playsam)

.include <bsd.dep.mk>

