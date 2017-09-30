#include <stdio.h>
#include <machine/soundcard.h>
#include "dsp.h"
#include "resampler.h"

char *DSP::Open()
{
    char *e = Player::Open();
    if (e == 0)
    {
        int fd = fileno(fp);
        if (ioctl(fd, SNDCTL_DSP_SPEED, &physrate)< 0)
  	    e =  "Error setting playback frequency";
	else
	{
            if (ioctl(fd, SNDCTL_DSP_SAMPLESIZE, &bits) < 0)
  	        e = "Error setting sample size";
	    //else
		//printf("Set DSP device to %d/%d\n", bits, physrate);
	}
    }
    return e;
}

void DSP::Play(class DigitalSource *s, int start, int length)
{
    if (physrate != SampleRate() || bits != SampleSize())
    {
	s = new Resampler(s->Clone(), SampleSize(), SampleRate(),
					bits, physrate);
        Player::Play(s, start, length);
        s->Delete();
    }
    else Player::Play(s, start, length);
}

DSP::~DSP()
{
}

