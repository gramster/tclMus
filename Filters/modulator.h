#ifndef MODULATOR_H
#define MODULATOR_H

#include "filter.h"

class Modulator : public Filter
{
  protected:
    DigitalSource *modulator;
    virtual int Combine(DigitalSource *carrier,
			DigitalSource *modulator, int p) const = 0;
  public:
    Modulator(DigitalSource *source_in = 0, DigitalSource *modulator_in)
	: Filter(source_in), modulator(modulator_in)
    {}
    //virtual int Length();
    virtual int Data(int p) const;
    virtual char *TypeName() const;
    virtual ~Modulator();
};

// Adder is like a two-channel mixer

class Adder : public Modulator
{
  protected:
    virtual int Combine(DigitalSource *carrier,
			DigitalSource *modulator, int p) const;
  public:
    Adder(DigitalSource *source_in = 0, DigitalSource *modulator_in)
	: Modulator(source_in, modulator_in)
    {}
    virtual char *TypeName() const;
    virtual ~Adder();
};

class Subtractor : public Modulator
{
  protected:
    virtual int Combine(DigitalSource *carrier,
			DigitalSource *modulator, int p) const;
  public:
    Subtractor(DigitalSource *source_in = 0, DigitalSource *modulator_in)
	: Modulator(source_in, modulator_in)
    {}
    virtual char *TypeName() const;
    virtual ~Subtractor();
};

class Multiplier : public Modulator
{
  protected:
    virtual int Combine(DigitalSource *carrier,
			DigitalSource *modulator, int p) const;
  public:
    Multiplier(DigitalSource *source_in = 0, DigitalSource *modulator_in)
	: Modulator(source_in, modulator_in)
    {}
    virtual char *TypeName() const;
    virtual ~Multiplier();
};

#endif

