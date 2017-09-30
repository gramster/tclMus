#ifndef CONSTANT_H
#define CONSTANT_H

#include "digsource.h"

class ConstantSource : public DigitalSource
{
  protected:
    int value;
  public:
    ConstantSource(int value_in, int length_in)
	: DigitalSource(), value(value_in)
    {
	length = length_in;
    }
    virtual int Data(int p) const;
    virtual char *TypeName() const;
    virtual ~ConstantSource();
};

//----------------------------------------------------------

#endif

