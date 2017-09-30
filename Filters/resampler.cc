#include "resampler.h"

int Resampler::Length() 
{
    if (length < 0)
    {
	unsigned long v = (unsigned long)source->Length();
	v *= (unsigned long)torate;
	v /= (unsigned long)fromrate;
    	length = (int)v;
    }
    return length;
}

int Resampler::Data(int p) const
{
    // the next two would probaly produce better results if interpolated when
    // resampling at a higher frequency
    p = (p * fromrate) / torate;
    int v = source->Data(p);
    // change the bitsize
    if (frombits > tobits)
	v >>= (frombits - tobits);
    else if (frombits < tobits)
	v <<= (tobits - frombits);
    return v;
}

char *Resampler::TypeName() const
{
    return "Resampler";
}

Resampler::~Resampler()
{
}


