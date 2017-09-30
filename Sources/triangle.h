#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "waveform.h"

class TriangleWaveForm : public WaveForm
{
  public:
    TriangleWaveForm(int freq_in, int length_in)
	: WaveForm(freq_in, length_in)
    { }
    virtual int Data(int p) const;
    virtual char *TypeName() const;
    virtual ~TriangleWaveForm();
};

#endif

