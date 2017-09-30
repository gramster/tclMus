#include <math.h>
#include "sawtooth.h"

int SawToothWaveForm::Data(int p) const
{
    if (p < 0 || p>=length) return 0;
    return (((p % wavelength) * amplitude) /  wavelength) - maxval;
}

char *SawToothWaveForm::TypeName() const
{
    return "SawToothWaveForm";
}

SawToothWaveForm::~SawToothWaveForm()
{}


