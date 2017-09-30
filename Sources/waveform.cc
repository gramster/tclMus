#include <math.h>
#include "square.h"

int WaveForm::Data(int p) const
{
    if (p < 0 || p>=length) return 0;
    return DigitalSource::Data(p % wavelength);
}

char *WaveForm::TypeName() const
{
    return "WaveForm";
}

WaveForm::~WaveForm()
{}


