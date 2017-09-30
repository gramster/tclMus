#ifndef SINEWAVE_H
#define SINEWAVE_H

#include "waveform.h"

class SineWaveForm : public WaveForm
{
    double factor;
  public:
	// remember middle C is 440Hz
    SineWaveForm(int freq_in, int length_in)
	: WaveForm(freq_in, length_in),
	  factor( (2.*3.1415927*((double)freq_in)) / ((double)SampleRate()))
    { }
    virtual int Data(int p) const;
    virtual char *TypeName() const;
    virtual ~SineWaveForm();
};

#endif

