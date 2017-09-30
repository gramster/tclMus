#ifndef MIXER_H
#define MIXER_H

#include "digsource.h"
#include "amplifier.h"

class Mixer : public DigitalSource
{
  protected:
    Amplifier *channels;
    int nchannels;
  public:
    Mixer(int nchannels_in);
    void SetChannelSource(int n, DigitalSource *s);
    void SetChannelLevel(int n, int level)
    {
	channels[n].SetLevel(level);
    }
    virtual int Data(int p) const;
    virtual int Length();
    virtual char *TypeName() const;
    virtual ~Mixer();
};

#endif

