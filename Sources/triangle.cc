#include <math.h>
#include "triangle.h"

int TriangleWaveForm::Data(int p) const
{
    if (p < 0 || p>=length) return 0;
    p = p % wavelength;
    int v = maxval - (p*maxval*2) / wavelength;
    if (p >= (wavelength/2))
	return v;
    else
	return -v;
}

char *TriangleWaveForm::TypeName() const
{
    return "TriangleWaveForm";
}

TriangleWaveForm::~TriangleWaveForm()
{}


