#ifndef FSHIFTER_H
#define FSHIFTER_H

#include "filter.h"

class FrequencyShifter : public Filter
{
  protected:
    int multiplier;
    int divider;
  public:
    FrequencyShifter(DigitalSource *source_in = 0,
			int multiplier_in = 1, int divider_in = 1)
	: Filter(source_in),
	  multiplier(multiplier_in),
	  divider(divider_in)
    { }
    virtual int Data(int p) const;
    virtual int Length();
    virtual char *TypeName() const;
    virtual ~FrequencyShifter();
};

#endif

