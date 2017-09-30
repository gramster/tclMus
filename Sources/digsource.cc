#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "digsource.h"

static int internal_sample_rate = 8000;
static int internal_sample_size = 8;
static int can_set_sample_params = 1;

int SampleRate()
{
    return internal_sample_rate;
}

int SampleSize()
{
    return internal_sample_size;
}

int SetSampleRate(int v)
{
    if (can_set_sample_params) internal_sample_rate = v;
    return internal_sample_rate;
}

int SetSampleSize(int v)
{
    if (can_set_sample_params) internal_sample_size = v;
    return internal_sample_size;
}

int DigitalSource::nextid = 0;
#ifdef LEAKCHECK
unsigned long DigitalSource::usevec[(max_sources+31)/32];
#endif

DigitalSource::DigitalSource(int length_in)
    : length(length_in),
      pos(0),
      id(nextid++),
      refcnt(1)
{
    can_set_sample_params = 0;
#ifdef LEAKCHECK
    usevec[id/32] |= 1l << (id%32);
#endif
}

DigitalSource *DigitalSource::Clone() 
{
    refcnt++;
    //printf("Clone: %s ID %d refcnt %d\n", TypeName(), id, refcnt);
    return this;
}

int DigitalSource::Delete()
{
    int rtn = --refcnt;
    //printf("Delete: %s ID %d refcnt %d\n", TypeName(), id, refcnt);
    if (rtn == 0) delete this;
    return rtn;
}

const DigitalSource *DigitalSource::Instrument() const
{
    return this;
}

int DigitalSource::Pitch() const
{
    return 0;
}

int DigitalSource::Data(int p) const
{
    (void)p;
    return 0;
}

int DigitalSource::Length()
{
    return (length != UnknownLength ? length : 0); // 0 for sanity
}

char *DigitalSource::TypeName() const
{
    return "DigitalSource";
}

char *DigitalSource::Name() const
{
    return TypeName();
}

void DigitalSource::WritePlotData(char *fname, int len)
{
    if (len < 0)
    {
	len = Length();
	if (len > 5*SampleRate()) len = 5*SampleRate(); // sanity
    }
    FILE *fp = fopen(fname, "w");
    if (fp)
    {
	for (int i = 0; i < len; i++)
	    fprintf(fp, "%d %d\n", i, Data((unsigned)i));
	fclose(fp);
    }
}

void DigitalSource::XPlot(int len)
{
    // very quick and dirty plotting
    WritePlotData("/tmp/pdata", len);
    FILE *fp = fopen("/tmp/pctrl", "w");
    if (fp)
    {
	fprintf(fp, "#!/usr/local/bin/gnuplot\n");
	fprintf(fp, "set terminal x11\n");
	fprintf(fp, "plot \"/tmp/pdata\" with lines\n");
	fprintf(fp, "pause -1\n");
	fclose(fp);
	chmod("/tmp/pctrl", 0777);
	system("/tmp/pctrl");
	unlink("/tmp/pctrl");
    }
    unlink("/tmp/pdata");
}

#ifdef LEAKCHECK
int DigitalSource::MaxSources()
{
    return max_sources;
}

int DigitalSource::InUse(int id)
{
    if (id < max_sources)
        return ((usevec[id/32] & (1l << (id%32))) != 0);
    else
	return 0; // sanity
}
#endif

DigitalSource::~DigitalSource()
{
    //printf("real delete of id %d\n", id);
#ifdef LEAKCHECK
    usevec[id/32] &= ~(1l << (id%32));
#endif
}

