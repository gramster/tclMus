#include "fshifter.h"

int FrequencyShifter::Data(int p) const
{
    return Filter::Data((p * divider) / multiplier);
}

int FrequencyShifter::Length()
{
    if (length < 0) length = ((Filter::Length() * multiplier) / divider);
    return length;
}

char *FrequencyShifter::TypeName() const
{
    return "FrequencyShifter";
}

FrequencyShifter::~FrequencyShifter()
{
}



