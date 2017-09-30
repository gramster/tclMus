#include "multiecho.h"

int MultiEcho::Length()
{
    if (length < 0)
    {
        int l = level;
        length = Filter::Length();
        while (l > 0)
        {
	    length += duration;
	    l = (l * level)/100;
        }
    }
    return length;
}

int MultiEcho::Data(int p) const
{
    int l = 100;
    int v = 0;
    while (l > 0 && p >= 0)
    {
	v += (l * Filter::Data(p)) / 100;
	l = (l * level) / 100;
	p -= duration;
    }
    return v;
}

char *MultiEcho::TypeName() const
{
    return "MultiEcho";
}

MultiEcho::~MultiEcho()
{
}


