#ifndef DSP_H
#define DSP_H

#include "player.h"

class DSP : public Player
{
    int physrate;
  public:
    DSP(int bits_in, int physrate_in)
	: Player("/dev/dsp", bits_in), physrate(physrate_in)
    {}
    virtual char *Open();
    virtual void Play(class DigitalSource *s, int start=0, int length=-1);
    virtual ~DSP();
};

#endif

