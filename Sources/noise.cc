#include <stdlib.h>
#include "noise.h"

int Noise::Data(int p) const
{
    (void)p;
    return (random() % amplitude) - maxval;
}

char *Noise::TypeName() const
{
    return "Noise";
}

Noise::~Noise()
{
}

