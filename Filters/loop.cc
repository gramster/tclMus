#include <assert.h>
#include "loop.h"

int Loop::Length()
{
    if (looplength < 0) looplength = source->Length();
    if (length < 0) length = (loops-1)*looplength + source->Length();
    return length;
}

int Loop::Data(int p) const
{
    int l = length, ll = looplength;
    if (l < 0) { ll = source->Length(); l = loops*ll; }
    if (p < 0 || p >= l) return 0;
    else
    {
	// This is a bit obscure, but we don't want sudden silence
	// at the end of the loop. This code carries over any sources
	// from previous loops that are still playing.

	int cnt = p / ll;		// the loop count
	if (cnt >= loops) cnt = loops-1;	// sanitise
	int pos = p - cnt * ll;
	int v = 0;
	while (cnt-- >= 0 && pos < source->Length())
	{
	    v += Filter::Data(pos);
	    pos += ll;
	}
	return v;
    }
}

char *Loop::TypeName() const
{
    return "Loop";
}

Loop::~Loop()
{
}


