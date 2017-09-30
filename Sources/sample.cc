#include <stdio.h>
#include <string.h>
#include "sample.h"

int Sample::Length()
{
    if (sf && length == UnknownLength) length = sf->Length();
    return DigitalSource::Length();
}

int Sample::Data(int p) const
{
    return sf ? sf->Data(p) : 0;
}

char *Sample::TypeName() const
{
    return "Sample";
}

Sample::~Sample()
{
}

