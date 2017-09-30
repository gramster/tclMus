#include <stdio.h>
#include <assert.h>
#include <machine/soundcard.h>
#include "digsource.h"
#include "player.h"

Player::Player(char *fname_in, int bits_in)
    : fp(0), bits(bits_in)
{
    fname = new char[strlen(fname_in)+1];
    if (fname) strcpy(fname, fname_in);
    int x = 1 << bits;
    midv = x >> 1;
    maxv = x - 1;
}

unsigned short Player::Sanitise(int v)
{
    v += midv;
    if (v > maxv) v = maxv;
    else if (v < 0) v = 0;
    return ((unsigned short)v)&0xFFFF;
}

char *Player::Open()
{
    fp = fopen(fname, "w");
    if (fp == 0) return "Cannot open player device";
    return 0;
}

void Player::Write(int v)
{
    assert (fp);
    unsigned short c = (unsigned short)((short)v);
    if (bits == 8)
    {
        c = Sanitise(v);
        fputc((char)(c&255), fp);
    }
    else
    {
        fputc((char)(c&255), fp);
        fputc((char)((c>>8)&255), fp);
    }
}

void Player::Close()
{
    if (fp) fclose(fp);
    fp = 0;
}

void Player::Play(DigitalSource *s, int start, int length)
{
    if (fp == 0) Open();
    assert(fp);
    int l = s->Length();
    if (length>=0 && l > (start+length)) l = start+length;
    //printf("Playing %s rate %d bits %d length %d at bits %d\n",
			//s->Name(), SampleRate(), SampleSize(), l, bits);
    for (int i = start; i < l; i++)
	Write(s->Data(i));
}

Player::~Player()
{
    Close();
    delete [] fname;
}


