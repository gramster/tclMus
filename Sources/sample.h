#ifndef SAMPLE_H
#define SAMPLE_H

#include "samplef.h"

class Sample : public DigitalSource
{
  protected:
    DigitalSource *sf;
  public:
    Sample(const char *sname = 0, int bits = 0, int rate = 0)
	: DigitalSource(), sf(0)
    {
	if (sname) sf = SampleBank::Get(sname, bits, rate);
    }
    char *Name() const
    {
	return sf ? sf->Name() : 0;
    }
    virtual int Length();
    virtual int Data(int p) const;
    virtual char *TypeName() const;
    virtual ~Sample();
};

#endif

