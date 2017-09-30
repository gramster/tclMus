#ifndef FILTER_H
#define FILTER_H

#include "digsource.h"

class Filter : public DigitalSource
{
  protected:
    DigitalSource *source;
  public:
    DigitalSource *Source() const
    {
	return source;
    }
    virtual DigitalSource *Clone();
    virtual int Delete();
    virtual void SetSource(DigitalSource *s=0);
    virtual DigitalSource *Instrument() const;
    virtual int Pitch() const;
    virtual int Length();
    virtual int Data(int p) const;
    virtual char *TypeName() const;
    virtual char *Name() const;

    Filter(DigitalSource *source_in = 0)
	: DigitalSource(), source(0)
    {
	SetSource(source_in);
    }
    virtual ~Filter();
};

#endif

