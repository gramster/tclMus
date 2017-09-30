#ifndef ENVELOPE_H
#define ENVELOPE_H

#include "filter.h"

class Envelope : public Filter
{
  protected:
    int scale;
    struct Operator {
	unsigned pos;
	unsigned level;
	Operator() : pos(0), level(100) {}
    } *op;
    int nops;
    int srclen;

    inline unsigned Pos(int o) const
    {
	return op[o].pos;
    }
    inline unsigned Level(int o) const
    {
	return op[o].level;
    }
    inline int XScale() const
    {
	return op[nops-1].pos;
    }
  public:
    Envelope(DigitalSource *source_in, int scale_in, int nops_in)
	: Filter(source_in), scale(scale_in), nops(nops_in)
    {
	op = new Operator[nops];
        srclen = source_in ? source_in->Length() : 0;
    }
    inline int NumOps() const
    {
	return nops;
    }
    void Set(int o, int pos, int level)
    {
	op[o].pos = pos;
	op[o].level = level;
    }
    virtual int Length();
    virtual int Data(int p) const;
    virtual char *TypeName() const;
    virtual ~Envelope();
};

class ClipEnvelope : public Envelope
{
  public:
    ClipEnvelope(DigitalSource *source_in,
		unsigned scale_in,
		unsigned clippoint)
	: Envelope(source_in, scale_in, 4)
    {
        Set(0, 0, 100);
        Set(1, clippoint-1, 100);
	Set(2, clippoint, 0);
	Set(3, scale, 0);
    }
    virtual char *TypeName() const;
    virtual ~ClipEnvelope();
};

class REnvelope : public Envelope
{
  public:
    REnvelope(DigitalSource *source_in,
		unsigned scale_in,
		unsigned release_end)
	: Envelope(source_in, scale_in, 3)
    {
        Set(0, 0, 100);
        Set(1, release_end, 0);
	Set(3, scale, 0);
    }
    virtual char *TypeName() const;
    virtual ~REnvelope();
};

class SREnvelope : public Envelope
{
  public:
    SREnvelope(DigitalSource *source_in,
		unsigned scale_in,
		unsigned sustain, unsigned release)
	: Envelope(source_in, scale_in, 4)
    {
        Set(0, 0, 100);
        Set(1, sustain, 100);
	Set(2, sustain+release, 0);
	Set(3, scale, 0);
    }
    virtual char *TypeName() const;
    virtual ~SREnvelope();
};

class ADSREnvelope : public Envelope
{
  public:
    ADSREnvelope(DigitalSource *source_in,
		 unsigned scale_in,
		 unsigned attack, unsigned decay, unsigned decaylevel,
		 unsigned sustain, unsigned release)
	: Envelope(source_in, scale_in, 6)
    {
        Set(0, 0, 0);
        Set(1, attack, 100);
	Set(2, attack+decay, decaylevel);
        Set(3, attack+decay+sustain, decaylevel);
	Set(4, attack+decay+sustain+release, 0);
	Set(5, scale, 0);
    }
    virtual char *TypeName() const;
    virtual ~ADSREnvelope();
};

#endif

