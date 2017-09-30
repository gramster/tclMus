#ifndef REVERSE_H
#define REVERSE_H

#include "filter.h"

class Reverse : public Filter
{
  public:
    Reverse(DigitalSource *source_in = 0)
	: Filter(source_in)
    {}
    virtual int Data(int p) const;
    virtual char *TypeName() const;
    virtual ~Reverse();
};

#endif

