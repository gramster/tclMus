#ifndef MULTIECHO_H
#define MULTIECHO_H

#include "filter.h"

class MultiEcho : public Filter
{
    int duration, level;
  public:
    MultiEcho(DigitalSource *source_in = 0, int duration_in = 0, int level_in = 0)
	: Filter(source_in), duration(duration_in), level(level_in)
    {}
    virtual int Length();
    virtual int Data(int p) const;
    virtual char *TypeName() const;
    virtual ~MultiEcho();
};

#endif

