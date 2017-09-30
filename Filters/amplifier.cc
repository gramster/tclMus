#include "amplifier.h"

int Amplifier::Length()
{
    if (length < 0)
    	length = level ? Filter::Length() : 0;
    return length;
}

int Amplifier::Data(int p) const
{
    int v = Filter::Data(p);
    v = (v * level) / 100;
#if 0
    // do clipping
    if (SampleSize() == 8)
    {
	if (v > 127) v = 127;
	else if (v < -127) v = -127;
    }
    else
    {
	if (v > 32767) v = 32767;
	else if (v < -32767) v = -32767;
    }
#endif
    return v;
}

char *Amplifier::TypeName() const
{
    return "Amplifier";
}

Amplifier::~Amplifier()
{
}


