#include "modulator.h"

int Modulator::Data(int p) const
{
    return Combine(source, modulator, p);
}

char *Modulator::TypeName() const
{
    return "Modulator";
}

Modulator::~Modulator()
{
}

int Adder::Combine(DigitalSource *carrier,
			DigitalSource *modulator, int p) const
{
    return carrier->Data(p) + modulator->Data(p);
}

char *Adder::TypeName() const
{
    return "Adder";
}

Adder::~Adder()
{
}

int Subtractor::Combine(DigitalSource *carrier,
			DigitalSource *modulator, int p) const
{
    return carrier->Data(p) - modulator->Data(p);
}

char *Subtractor::TypeName() const
{
    return "Subtractor";
}

Subtractor::~Subtractor()
{
}

int Multiplier::Combine(DigitalSource *carrier,
			DigitalSource *modulator, int p) const
{
    int mv = (1 << (SampleSize()-1)) - 1;
    return (carrier->Data(p) * modulator->Data(p)) / mv;
}

char *Multiplier::TypeName() const
{
    return "Multiplier";
}

Multiplier::~Multiplier()
{
}


