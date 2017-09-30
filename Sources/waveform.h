#ifndef WAVEFORM_H
#define WAVEFORM_H

#include "digsource.h"

class WaveForm : public DigitalSource
{
  protected:
    int freq;
    int amplitude;
    int maxval;
    int wavelength;
  public:
    WaveForm(int freq_in, int length_in)
	: DigitalSource(length_in), freq(freq_in)
    {
        amplitude = 1<<SampleSize();
	maxval = (amplitude/2)-1;
        wavelength = (SampleRate() / freq);
    }
    virtual int Data(int p) const;
    virtual char *TypeName() const;
    virtual ~WaveForm();
};

#endif

