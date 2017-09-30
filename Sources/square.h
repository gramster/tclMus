#ifndef SQUARE_H
#define SQUARE_H

#include "waveform.h"

class SquareWaveForm : public WaveForm
{
  public:
    SquareWaveForm(int freq_in, int length_in)
	: WaveForm(freq_in, length_in)
    { }
    virtual int Data(int p) const;
    virtual char *TypeName() const;
    virtual ~SquareWaveForm();
};

#endif

