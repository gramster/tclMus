#ifndef ECHO_H
#define ECHO_H

#include "filter.h"

class Echo : public Filter
{
    int duration, level;
  public:
    Echo(DigitalSource *source_in, int duration_in, int level_in)
	: Filter(source_in), duration(duration_in), level(level_in)
    {}
    virtual int Length();
    virtual int Data(int p) const;
    virtual char *TypeName() const;
    virtual ~Echo();
};

#endif


