#include "echo.h"

int Echo::Length()
{
    if (length < 0)
	length = Filter::Length()+duration;
    return length;
}

int Echo::Data(int p) const
{
    if (p >= duration)
        return source->Data(p) + (level * source->Data(p-duration)) / 100;
    else
        return source->Data(p);
}

char *Echo::TypeName() const
{
    return "Echo";
}

Echo::~Echo()
{
}

