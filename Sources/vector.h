#ifndef VECTOR_H
#define VECTOR_H

#include "digsource.h"

class Vector : public DigitalSource
{
  protected:
    int bits;
    int rate;
    unsigned char *data;
    unsigned nbytes;
    unsigned char *Data() const
    {
	return data;
    }
    void Set8BitData(int p, int v);
    void Set16BitData(int p, int v);
  public:
    Vector(int length_in, int bits_in, int rate_in);
    void Init(int length_in);
    void SetData(int p, int v);
    virtual int Length();
    virtual int Data(int p) const;
    virtual char *TypeName() const;
    virtual ~Vector();
};

#endif

