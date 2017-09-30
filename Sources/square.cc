#include <math.h>
#include "square.h"

int SquareWaveForm::Data(int p) const
{
    if (p < 0 || p>=length) return 0;
    p = p % wavelength;
    if (p >= (wavelength/2)) return maxval;
    else return -maxval;
}

char *SquareWaveForm::TypeName() const
{
    return "SquareWaveForm";
}

SquareWaveForm::~SquareWaveForm()
{}


