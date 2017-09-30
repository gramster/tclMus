#include <math.h>
#include "transposer.h"

Transposer::Transposer(DigitalSource *source_in, int note, int octave)
	: FrequencyShifter(source_in,
			   1024,
			   (int)(.5+1024.* pow(1.059463,
					    (double)(pitch=note+12*octave-24))))
{
}

int Transposer::Pitch() const
{
    return pitch;
}

int Transposer::Data(int p) const
{
    return FrequencyShifter::Data(p);
}

int Transposer::Length()
{
    return FrequencyShifter::Length();
}

char *Transposer::TypeName() const
{
    return "Transposer";
}

Transposer::~Transposer()
{
}


