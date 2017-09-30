#ifndef OFFSET_H
#define OFFSET_H

#include "filter.h"

class Offset : public Filter
{
  protected:
    int offset;
  public:
    Offset(DigitalSource *source_in = 0, int offset_in = 0)
	: Filter(source_in), offset(offset_in)
    {}
    virtual int Data(int p) const;
    virtual char *TypeName() const;
    virtual ~Offset();
};

#endif

