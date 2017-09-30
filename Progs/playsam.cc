#include <stdio.h>
#include <stdlib.h>
#include "sample.h"
#include "sinewave.h"
#include "envelope.h"
#include "loop.h"
#include "transposer.h"
#include "noise.h"
#include "dsp.h"

void PlayDigitalSource(DigitalSource *s)
{
    DSP p(8, 8000);
    p.Play(s);
    s->XPlot();
}

void PlaySample(char *sname, int rate, int playfreq, int bits)
{
    (void)playfreq;
    Sample *s = new Sample(sname, bits, rate);
    if (s)
    {
	puts(s->Name());
	PlayDigitalSource(s);
    	delete s;
    }
}

void PlaySamples(char *fname, int playfreq, int bits)
{
    FILE *fp = fopen(fname, "r");
    if (fp)
    {
	while (!feof(fp))
	{
	    char buff[128], sname[128];
	    int rate;
	    if (fgets(buff, sizeof(buff)-1, fp) == 0) break;
	    buff[sizeof(buff)-1] = 0;
	    if (sscanf(buff, "%127s%d", sname, &rate) == 2)
		PlaySample(sname, rate, playfreq, bits);
	}
	fclose(fp);
    }
}

int main(int argc, char **argv)
{
#if 1
    (void)argc; (void)argv;
    ADSREnvelope *e = new ADSREnvelope(new SineWaveForm(36, 360),
					360, 80, 30, 60, 140, 50);
    //ADSREnvelope *e = new ADSREnvelope(new Noise(800),
					//360, 80, 30, 60, 140, 50);
    Loop *l = new Loop(e, 10);
    for (int octave = 2; octave < 8; octave++)
    {
	Transposer *t = new Transposer(l->Clone(), 0, octave);
	printf("%d\n", octave);
        PlayDigitalSource(t);
	getchar();
        delete t;
    }
    delete l;
#else
    int playfreq = 8000;
    int bits = 8;
    //int playfreq = 16000;
    //int bits = 16;
    if (argc == 3) PlaySample(argv[1], atoi(argv[2]), playfreq, bits);
    else if (argc == 2) PlaySamples(argv[1], playfreq, bits);
    else if (argc == 1) PlaySamples("samplelist", playfreq, bits);
#endif
    return 0;
}

