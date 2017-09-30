#ifndef NOISE_H
#define NOISE_H

#include "waveform.h"

class Noise : public WaveForm
{
  public:
    Noise(int length_in)
	: WaveForm(SampleRate(), length_in)
    { }
    virtual int Data(int p) const;
    virtual char *TypeName() const;
    virtual ~Noise();
};

#endif

