#ifndef SAWTOOTH_H
#define SAWTOOTH_H

#include "waveform.h"

class SawToothWaveForm : public WaveForm
{
  public:
	// remember middle C is 440Hz
    SawToothWaveForm(int freq_in, int length_in)
	: WaveForm(freq_in, length_in)
    { }
    virtual int Data(int p) const;
    virtual char *TypeName() const;
    virtual ~SawToothWaveForm();
};

#endif

