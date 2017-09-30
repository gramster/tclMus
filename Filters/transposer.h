#ifndef TRANSPOSER_H
#define TRANSPOSER_H

#include "fshifter.h"

class Transposer : public FrequencyShifter
{
  protected:
    int pitch;
  public:
    Transposer(DigitalSource *source_in = 0, int note_in = 0, int octave_in =2);
    virtual int Pitch() const;
    virtual int Data(int p) const;
    virtual int Length();
    virtual char *TypeName() const;
    virtual ~Transposer();
};

#endif

