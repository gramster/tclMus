#include "resampler.h"
#include "mixer.h"

Mixer::Mixer(int nchannels_in)
    : DigitalSource(), nchannels(nchannels_in)
{
    channels = new Amplifier[nchannels];
}

void Mixer::SetChannelSource(int n, DigitalSource *s)
{
    channels[n].SetSource(s);
}

int Mixer::Data(int p) const
{
    int v = 0;
    for (int i = 0; i < nchannels; i++)
	v += channels[i].Data(p);
    return v;
}

int Mixer::Length()
{
    if (length < 0)
    {
	length = 0;
    	for (int i = 0; i < nchannels; i++)
    	{
	    int l = channels[i].Length();
	    if (l > length) length = l;
    	}
    }
    return length;
}

char *Mixer::TypeName() const
{
    return "Mixer";
}

Mixer::~Mixer()
{
    delete [] channels;
}

