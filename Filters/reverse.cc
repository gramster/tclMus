#include "reverse.h"

int Reverse::Data(int p) const
{
    return Filter::Data(source->Length()-p-1);
}

char *Reverse::TypeName() const
{
    return "Reverse";
}

Reverse::~Reverse()
{
}


