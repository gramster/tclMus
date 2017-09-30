#ifndef RESAMPLER_H
#define RESAMPLER_H

#include "filter.h"

class Resampler : public Filter
{
    int frombits, fromrate, tobits, torate;
  public:
    Resampler(DigitalSource *source_in, int fbits_in, int frate_in,
					int tbits_in, int trate_in)
	: Filter(source_in),
	  frombits(fbits_in), fromrate(frate_in),
	  tobits(tbits_in), torate(trate_in)
    {}
    virtual int Length();
    virtual int Data(int p) const;
    virtual char *TypeName() const;
    virtual ~Resampler();
};

#endif

