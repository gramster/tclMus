#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "vector.h"

Vector::Vector(int length_in, int bits_in, int rate_in)
    : DigitalSource(), bits(bits_in), rate(rate_in), data(0), nbytes(0)
{
    Init(length_in);
}

void Vector::Init(int length_in)
{
    delete [] data;
    length = length_in;
    if (length > 0)
    {
	if (bits == 8)
	    nbytes = length;
	else
	    nbytes = length<<1;
	data = new unsigned char[nbytes];
    }
    else
    {
	data = 0;
	nbytes = 0;
    }
}

void Vector::Set8BitData(int p, int v)
{
    data[p] = (unsigned char)((char)v);
}

void Vector::Set16BitData(int p, int v)
{
    unsigned short sv = ((unsigned short)((short)v));
    data[p*2+1] = (unsigned char)(sv>>8);
    data[p*2] = (unsigned char)(v & 0xff);
}

void Vector::SetData(int p, int v)
{
    if (bits == 8) Set8BitData(p, v);
    else Set16BitData(p, v);
    if(v != Data(p)) printf("v = %d Data(p)=%d\n", v, Data(p));
}

int Vector::Length()
{
    if (length < 0)
    {
        if (bits == 8) length = nbytes;
        else length = nbytes>>1;
    }
    return length;
}

int Vector::Data(int p) const
{
    if (p < 0 || p>= length) return 0;
    if (bits==8) return (int)((char)data[p]);
    else return (int)((short)((data[(p<<1)+1]<<8)|data[(p<<1)]));
}

char *Vector::TypeName() const
{
    return "Vector";
}

Vector::~Vector()
{
    delete [] data;
}

