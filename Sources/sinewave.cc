#include <math.h>
#include "sinewave.h"

int SineWaveForm::Data(int p) const
{
    if (p < 0 || p>=length) return 0;
    return (int)( ((double)maxval) * sin(factor * (double)p) );
}

char *SineWaveForm::TypeName() const
{
    return "SineWaveForm";
}

SineWaveForm::~SineWaveForm()
{}


