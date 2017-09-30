#ifndef AMPLIFIER_H
#define AMPLIFIER_H

#include "filter.h"

class Amplifier : public Filter
{
  protected:
    int level; // integer percentage
  public:
    Amplifier(DigitalSource *source_in = 0, int level_in = 100)
	: Filter(source_in), level(level_in)
    {}
    inline void SetLevel(int l)
    {
	level = l;
    }
    virtual int Length();
    virtual int Data(int p) const;
    virtual char *TypeName() const;
    virtual ~Amplifier();
};

#endif

