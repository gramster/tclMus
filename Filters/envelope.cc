
#include "envelope.h"

int Envelope::Data(int p) const
{
    if (p < 0 || p >= srclen) return 0;
    int v = Filter::Data(p);

    p = (p * scale) / srclen;
    for (int o = nops-1; --o >= 0; )
	if (p >= (int)op[o].pos)
	{
	    int x = p - op[o].pos;
	    int dx = op[o+1].pos - op[o].pos;
	    int dy = (op[o+1].level - op[o].level); // -100..+100
	    int y = op[o].level + (x * dy) / dx;
	    v = (v * y) / 100;
	    break;
	}
    return v;
}

int Envelope::Length()
{
    if (scale == 0) return length = 0;
    int p = nops;
    while (p > 0 && op[p-1].level==0) p--;
    length = (srclen * op[p].pos) / scale;
    return length;
}

char *Envelope::TypeName() const
{
    return "Envelope";
}

Envelope::~Envelope()
{
    delete [] op;
}

char *ClipEnvelope::TypeName() const
{
    return "ClipEnvelope";
}

ClipEnvelope::~ClipEnvelope()
{}

char *REnvelope::TypeName() const
{
    return "REnvelope";
}

REnvelope::~REnvelope()
{}

char *SREnvelope::TypeName() const
{
    return "SREnvelope";
}

SREnvelope::~SREnvelope()
{}

char *ADSREnvelope::TypeName() const
{
    return "ADSREnvelope";
}

ADSREnvelope::~ADSREnvelope()
{}

