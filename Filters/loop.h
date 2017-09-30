#ifndef LOOP_H
#define LOOP_H

#include "filter.h"

class Loop : public Filter
{
  protected:
    int loops;	// all DigitalSources must have finite length...
    int looplength;
  public:
    Loop(DigitalSource *source_in = 0, int loops_in = 1, int length_in = -1)
	: Filter(source_in), loops(loops_in), looplength(length_in)
    { }
    virtual int Length();
    virtual int Data(int p) const;
    virtual char *TypeName() const;
    virtual ~Loop();
};

#endif

