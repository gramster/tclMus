#ifndef PLAYER_H
#define PLAYER_H

class Player
{
  protected:
    char *fname;
    FILE *fp;
    int bits;
    int midv, maxv;
    unsigned short Sanitise(int v);
  public:
    Player(char *fname, int bits_in = 8);
    virtual char *Open();
    void Write(int v);
    void Close();
    virtual void Play(class DigitalSource *s, int start=0, int length=-1);
    void PlaySection(class DigitalSource *s, int start, int length);
    virtual ~Player();
};

#endif

