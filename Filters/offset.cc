#include "offset.h"

int Offset::Data(int p) const
{
    return Filter::Data(p+offset);
}

char *Offset::TypeName() const
{
    return "Offset";
}

Offset::~Offset()
{
}


