SUBDIR=Drivers Sources Filters Progs Script
CHECKINS+=Makefile TODO

.include <bsd.subdir.mk>

checkin:
	@for i in $(SUBDIR); do (cd $$i; pwd; make checkin); done
	-ci -l $(CHECKINS)
	    


