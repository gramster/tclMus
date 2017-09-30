#include "filter.h"

DigitalSource *Filter::Clone() 
{
    if (source) (void)source->Clone();
    return DigitalSource::Clone();
}

int Filter::Delete()
{
    if (source) 
    {
	source->Delete();
	if (refcnt<=1) source = 0;
    }
    return DigitalSource::Delete();
}

DigitalSource *Filter::Instrument() const
{
    return source;
}

int Filter::Pitch() const
{
    return source ? source->Pitch() : 0;
}

int Filter::Length()
{
    if (length < 0) length = source ? source->Length() : 0;
    return length;
}

int Filter::Data(int p) const
{
    return source ? source->Data(p) : 0; // default
}

char *Filter::TypeName() const
{
    return "Filter";
}

char *Filter::Name() const
{
    return source ? source->Name() : TypeName();
}

void Filter::SetSource(DigitalSource *s)
{
    length = -1;
    if (source) source->Delete();
    source = s;
}

Filter::~Filter()
{
    if (source) source->Delete();
}


