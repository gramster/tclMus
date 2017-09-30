// Tcl extensions:
//
// ROOT SOURCES:
//
// makesound noise <length>
// makesound constant <value> <length>
// makesound sinewave <freq> <length>
// makesound squarewave <freq> <length>
// makesound triwave <freq> <length>
// makesound sawwave <freq> <length>
//
// makesound sample <filename> [ <bits> <rate> ]
//
// FILTER PIPE SOURCES:
//
// makefilter reverse <sourcename> reverse
// makefilter offset <sourcename> offset <count>
// makefilter loop <sourcename> loop <count>
// makefilter echo <sourcename> echo <length> <level>
// makefilter multiecho <sourcename> multiecho <length> <level>
// makefilter amplify <sourcename> <volume>
// makefilter freqshift <sourcename> <multiplier> <divider>
// makefilter transpose <sourcename> <note> <octave>
// makefilter clipenvelope <sourcename> <scale> <release_endpoint>
// makefilter renvelope <sourcename> <scale> <release_endpoint>
// makefilter srenvelope <sourcename> <scale> <sustain> <release>
// makefilter adsrenvelope <sourcename> <scale> <attack> <decay> 
//				<decaylevel> <sustain> <release>
//
// modulate <sourcename> add <sourcename>
// modulate <sourcename> subtract <sourcename>
// modulate <sourcename> multiply <sourcename>
//
// MIXING:
//
// makemixer <nchannels>
// setinput <mixername> <channelnum> <sourcename>
// setvolume <mixername> <channelnum> <volume>
//
// SEQUENCING:
//
// makesequence ( multi | piano | retrigger )
// addsource <sequencename> <sourcename> <start>
//
// MEMORY MANAGEMENT:
//
// clonesound <name>...		Increment refcounts
// destroysound <name>...	Decrement refcounts and free if zero
// showsounds			Show currently allocated sounds
//
// MISC
//
// playsound <bits> <rate> <sourcename>...
// plotsound <sourcename>...
// expandsound <sourcename>	Create a sample from any source
// savesound (raw8 | raw16) <sourcename> <filename>
// soundrate			Return compiled-in internal rate
// soundbits
//

#include <sys/stat.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "sample.h"
#include "noise.h"
#include "sinewave.h"
#include "square.h"
#include "triangle.h"
#include "sawtooth.h"
#include "constant.h"
#include "offset.h"
#include "loop.h"
#include "reverse.h"
#include "echo.h"
#include "multiecho.h"
#include "fshifter.h"
#include "transposer.h"
#include "amplifier.h"
#include "resampler.h"
#include "envelope.h"
#include "modulator.h"
#include "mixer.h"
#include "sequence.h"
#include "dsp.h"
#include "tcl.h"

Tcl_HashTable dstable;

DigitalSource *GetDSource(char *name)
{
    Tcl_HashEntry *entryPtr = Tcl_FindHashEntry(&dstable, name);
    if (entryPtr)
	return (DigitalSource *)Tcl_GetHashValue(entryPtr);
    else
	return 0;
}

DigitalSource *GetDSource(int id)
{
    char nm[16];
    sprintf(nm, "ds%d", id);
    return GetDSource(nm);
}

static char *AddDSource(DigitalSource *ds)
{
    Tcl_HashEntry *entryPtr;
    int isnew;
    static char nm[16];
    sprintf(nm, "ds%d", ds->ID());
    entryPtr = Tcl_CreateHashEntry(&dstable, nm, &isnew);
    assert(isnew);
    Tcl_SetHashValue(entryPtr, ds);
    return nm;
}

int DSSoundCmd(ClientData *cdata, Tcl_Interp *interp,
		int argc, char *argv[])
{
    DigitalSource *ds = 0;
    if (strcmp(argv[1], "sample") == 0)
    {
	if (argc == 5)
	    ds = new Sample(argv[2], atoi(argv[3]), atoi(argv[4]));
	else if (argc == 3)
	    ds = new Sample(argv[2]);
    }
    else if (argc == 3 && strcmp(argv[1], "noise") == 0)
    {
	ds = new Noise(atoi(argv[2]));
    }
    else if (argc == 4 && strcmp(argv[1], "constant") == 0)
    {
	ds = new ConstantSource(atoi(argv[2]), atoi(argv[3]));
    }
    else if (argc == 4 && strcmp(argv[1], "sinewave") == 0)
	ds = new SineWaveForm(atoi(argv[2]), atoi(argv[3]));
    else if (argc == 4 && strcmp(argv[1], "squarewave") == 0)
	ds = new SquareWaveForm(atoi(argv[2]), atoi(argv[3]));
    else if (argc == 4 && strcmp(argv[1], "triwave") == 0)
	ds = new TriangleWaveForm(atoi(argv[2]), atoi(argv[3]));
    else if (argc == 4 && strcmp(argv[1], "sawwave") == 0)
	ds = new SawToothWaveForm(atoi(argv[2]), atoi(argv[3]));
    if (ds)
    {
	strcpy(interp->result, AddDSource(ds));
        return TCL_OK;
    }
    else
    {
	strcpy(interp->result, "");
        return TCL_OK; // 4 now
    }
}

int DSMakeMixerCmd(ClientData *cdata, Tcl_Interp *interp,
			int argc, char *argv[])
{
    if (argc == 2)
    {
	DigitalSource *ds=new Mixer(atoi(argv[1]));
	strcpy(interp->result, AddDSource(ds));
    	return TCL_OK; // 4 now
    }
    else
    {
	strcpy(interp->result, "");
    	return TCL_OK; // 4 now
    }
}

int DSMakeSeqCmd(ClientData *cdata, Tcl_Interp *interp,
			int argc, char *argv[])
{
    if (argc == 2)
    {
	DigitalSource *ds;
	if (strcmp(argv[1], "multi") == 0)
	    ds = new Sequence();
	else if (strcmp(argv[1], "piano") == 0)
	    ds = new PianoModeSequence();
	else if (strcmp(argv[1], "retrigger") == 0)
	    ds = new RetriggerModeSequence();
	if (ds) strcpy(interp->result, AddDSource(ds));
    	return TCL_OK; // 4 now
    }
    else
    {
	strcpy(interp->result, "");
    	return TCL_OK; // 4 now
    }
}

int DSAddSourceCmd(ClientData *cdata, Tcl_Interp *interp,
			int argc, char *argv[])
{
    if (argc == 4)
    {
	DigitalSource *s = GetDSource(argv[1]);
	if (s && strcmp(s->TypeName(), "Sequence")==0)
	{
	    ((Sequence*)s)->AddSource(GetDSource(argv[2]), atoi(argv[3]));
	}
    	return TCL_OK; // 4 now
    }
    else
    {
    	return TCL_OK; // 4 now
    }
}

int DSSetInputCmd(ClientData *cdata, Tcl_Interp *interp,
			int argc, char *argv[])
{
    if (argc == 4)
    {
	DigitalSource *m = GetDSource(argv[1]);
	if (m && strcmp(m->TypeName(), "Mixer")==0)
	{
	    ((Mixer*)m)->SetChannelSource(atoi(argv[2])-1, GetDSource(argv[3]));
	}
    	return TCL_OK;
    }
    else
    {
    	return TCL_OK; // 4 now
    }
}

int DSSetVolumeCmd(ClientData *cdata, Tcl_Interp *interp,
			int argc, char *argv[])
{
    if (argc == 4)
    {
	DigitalSource *m = GetDSource(argv[1]);
	if (m && strcmp(m->TypeName(), "Mixer")==0)
	{
	    ((Mixer*)m)->SetChannelLevel(atoi(argv[2])-1, atoi(argv[3]));
	}
    	return TCL_OK;
    }
    else
    {
    	return TCL_OK; // 4 now
    }
}

int DSFilterCmd(ClientData *cdata, Tcl_Interp *interp,
		int argc, char *argv[])
{
    DigitalSource *ds = 0;
    if (argc >= 3)
    {
	DigitalSource *src = GetDSource(argv[2]);
	if ((argc == 4 || argc == 5) && strcmp(argv[1], "loop") == 0)
	{
	    if (argc == 4)
	        ds = new Loop(src, atoi(argv[3]));
	    else
	        ds = new Loop(src, atoi(argv[3]), atoi(argv[4]));
	}
	else if (argc == 4 && strcmp(argv[1], "offset") == 0)
	{
	    ds = new Offset(src, atoi(argv[3]));
	}
	else if (argc == 5 && strcmp(argv[1], "echo") == 0)
	{
	    ds = new Echo(src, atoi(argv[3]), atoi(argv[4]));
	}
	else if (argc == 5 && strcmp(argv[1], "multiecho") == 0)
	{
	    ds = new MultiEcho(src, atoi(argv[3]), atoi(argv[4]));
	}
	else if (argc == 4 && strncmp(argv[1], "amplif", 6) == 0)
	{
	    ds = new Amplifier(src, atoi(argv[3]));
	}
	//else if (argc == 5 && strncmp(argv[1], "resamp", 6) == 0)
	//{
	    //ds = new Resampler(src, atoi(argv[3]), atoi(argv[4]));
	//}
	//else if (strcmp(argv[1], "envelope") == 0)
	//{
		//ds = new Envelope(src);
	//}
	else if (argc == 5 && strncmp(argv[1], "clipenv", 7) == 0)
	{
	    ds = new ClipEnvelope(src, atoi(argv[3]), atoi(argv[4]));
	}
	else if (argc == 5 && strncmp(argv[1], "renv", 4) == 0)
	{
	    ds = new REnvelope(src, atoi(argv[3]), atoi(argv[4]));
	}
	else if (argc == 6 && strncmp(argv[1], "srenv", 5) == 0)
	{
	    ds = new SREnvelope(src, atoi(argv[3]), atoi(argv[4]),
					atoi(argv[5]));
	}
	else if (argc == 9 && strncmp(argv[1], "adsrenv", 7) == 0)
	{
	    ds = new ADSREnvelope(src, atoi(argv[3]), atoi(argv[4]),
					atoi(argv[5]), atoi(argv[6]),
					atoi(argv[7]), atoi(argv[8]));
	}
	else if (argc == 5 && strncmp(argv[1], "transpose", 9) == 0)
	{
	    ds = new Transposer(src, atoi(argv[3]), atoi(argv[4]));
	}
	else if (argc == 5 && strncmp(argv[1], "fshift", 6) == 0)
	{
	    ds = new FrequencyShifter(src, atoi(argv[3]), atoi(argv[4]));
	}
	else if (strcmp(argv[1], "reverse") == 0)
	{
	    ds = new Reverse(src);
	}
    }
    if (ds)
    {
	strcpy(interp->result, AddDSource(ds));
        return TCL_OK;
    }
    else
    {
        return TCL_OK; // 4 now
    }
}

int DSModulateCmd(ClientData *cdata, Tcl_Interp *interp,
		int argc, char *argv[])
{
    DigitalSource *ds = 0;
    if (argc == 4)
    {
	DigitalSource *src = GetDSource(argv[1]);
	DigitalSource *mod = GetDSource(argv[3]);
	if (strcmp(argv[2], "add") == 0)
	    ds = new Adder(src, mod);
	else if (strcmp(argv[2], "subtract") == 0)
	    ds = new Subtractor(src, mod);
	else if (strcmp(argv[2], "multiply") == 0)
	    ds = new Multiplier(src, mod);
    }
    if (ds)
    {
	strcpy(interp->result, AddDSource(ds));
        return TCL_OK;
    }
    else
    {
        return TCL_OK; // 4 now
    }
}

int DSCloneCmd(ClientData *cdata, Tcl_Interp *interp,
		int argc, char *argv[])
{
    if (argc == 2)
    {
	DigitalSource *ds = GetDSource(argv[1]);
	if (ds)
	{
	    ds->Clone();
            sprintf(interp->result, "ds%d", ds->ID());
	}
    }
    return TCL_OK;
}

int DSDestroyCmd(ClientData *cdata, Tcl_Interp *interp,
		int argc, char *argv[])
{
    int i;
    for (i = 1; i < argc; i++)
    {
        Tcl_HashEntry *entryPtr = Tcl_FindHashEntry(&dstable, argv[i]);
	if (entryPtr)
	{
	    DigitalSource *ds = (DigitalSource *)Tcl_GetHashValue(entryPtr);
	    Tcl_DeleteHashEntry(entryPtr);
	    ds->Delete();
	}
    }
    return TCL_OK;
}

int DSPlaySectionCmd(ClientData *cdata, Tcl_Interp *interp,
		int argc, char *argv[])
{
    int i;
    if (argc == 4 && isdigit(argv[1][0]) && isdigit(argv[2][0]))
    {
	DigitalSource *ds = GetDSource(argv[3]);
	if (ds)
	{
            DSP p(SampleSize(), SampleRate());
	    p.Play(ds, atoi(argv[1]), atoi(argv[2]));
	}
    }
    else if (argc == 6 && isdigit(argv[1][0]) && isdigit(argv[2][0]) &&
    		isdigit(argv[3][0]) && isdigit(argv[4][0]))
    {
	DigitalSource *ds = GetDSource(argv[5]);
	if (ds)
	{
            DSP p(atoi(argv[1]), atoi(argv[2]));
	    p.Play(ds, atoi(argv[3]), atoi(argv[4]));
	}
    }
}

int DSPlayCmd(ClientData *cdata, Tcl_Interp *interp,
		int argc, char *argv[])
{
    int i;
    if (argc > 3 && isdigit(argv[1][0]))
    {
        DSP p(atoi(argv[1]), atoi(argv[2]));
        for (i = 3; i < argc; i++)
        {
	    DigitalSource *ds = GetDSource(argv[i]);
	    if (ds) p.Play(ds);
        }
    }
    else
    {
        for (i = 1; i < argc; i++)
        {
	    DigitalSource *ds = GetDSource(argv[i]);
	    if (ds)
	    {
                DSP p(SampleSize(), SampleRate());
	        p.Play(ds);
	    }
        }
    }
    return TCL_OK;
}

int DSPlotCmd(ClientData *cdata, Tcl_Interp *interp,
		int argc, char *argv[])
{
    int i;
    if (argc == 2)
    {
	DigitalSource *ds = GetDSource(argv[1]);
	ds->XPlot();
    }
    else
    {
        FILE *gfp = fopen("/tmp/pctrl", "w");
        if (gfp)
        {
	    fprintf(gfp, "#!/usr/local/bin/gnuplot\n");
	    fprintf(gfp, "set terminal x11\n");
	    fprintf(gfp, "plot ");
            for (i = 1; i < argc; i++)
    	    {
	        DigitalSource *ds = GetDSource(argv[i]);
	        if (ds)
	        {
		    char fname[32];
		    sprintf(fname, "/tmp/%s", argv[i]);
		    ds->WritePlotData(fname);
		    if (i > 1) fprintf(gfp, ",");
		    fprintf(gfp, "\"%s\"", fname);
	        }
	    }
	    fprintf(gfp, " with lines\n");
	    fprintf(gfp, "pause -1\n");
	    fclose(gfp);
	    chmod("/tmp/pctrl", 0777);
	    system("/tmp/pctrl");
	    unlink("/tmp/pctrl");
            for (i = 1; i < argc; i++)
	    {
	        DigitalSource *ds = GetDSource(argv[i]);
	        if (ds)
	        {
		    char fname[32];
		    sprintf(fname, "/tmp/%s", argv[i]);
		    unlink(fname);
	        }
	    }
	}
    }
    return TCL_OK;
}

int DSLengthCmd(ClientData *cdata, Tcl_Interp *interp,
		int argc, char *argv[])
{
    if (argc == 2)
    {
	DigitalSource *ds = GetDSource(argv[1]);
	sprintf(interp->result, "%d", ds ? ds->Length() : 0);
    }
    return TCL_OK;
}

int DSRateCmd(ClientData *cdata, Tcl_Interp *interp,
		int argc, char *argv[])
{
    if (argc == 2)
	SetSampleRate(atoi(argv[1]));
    sprintf(interp->result, "%d", SampleRate());
    return TCL_OK;
}

int DSBitsCmd(ClientData *cdata, Tcl_Interp *interp,
		int argc, char *argv[])
{
    if (argc == 2)
	SetSampleSize(atoi(argv[1]));
    sprintf(interp->result, "%d", SampleSize());
    return TCL_OK;
}

int DSSaveCmd(ClientData *cdata, Tcl_Interp *interp,
		int argc, char *argv[])
{
    if (argc == 4)
    {
	assert(strcmp(argv[1], "raw8") == 0); // for now all we support
	DigitalSource *ds = GetDSource(argv[2]);
	if (ds)
	{
	    FILE *fp = fopen(argv[3], "w");
	    if (fp)
	    {
		int l = ds->Length();
		for (int i = 0; i < l; i++)
		{
		    unsigned char c = (unsigned char)ds->Data(i);
		    c += 128;
		    fputc(c, fp);
		}
		fclose(fp);
	    }
	}
    }
    return TCL_OK;
}

int DSExpandCmd(ClientData *cdata, Tcl_Interp *interp,
		int argc, char *argv[])
{
    if (argc == 2)
    {
	DigitalSource *ds = GetDSource(argv[1]);
	if (ds)
	{
	    int l = ds->Length();
	    Vector *v = new Vector(l, SampleSize(), SampleRate());
	    for (int i = 0; i < l; i++)
		v->SetData(i, ds->Data(i));
	    strcpy(interp->result, AddDSource(v));
	}
    }
    return TCL_OK;
}

#ifdef LEAKCHECK
int DSShowUsedCmd(ClientData *cdata, Tcl_Interp *interp,
		int argc, char *argv[])
{
    int i;
    for (i = 0; i < DigitalSource::MaxSources(); i++)
	if (DigitalSource::InUse(i))
	{
            Tcl_HashEntry *entryPtr;
            sprintf(interp->result, "ds%d", i);
	    DigitalSource *ds = GetDSource(interp->result);
	    if (ds)
		printf("Id %d %s\n", i, ds->TypeName());
	    else
	    {
		DigitalSource *s = SampleBank::Get(i);
		if (s)
		    printf("Id %d samplebank %s\n", i, s->Name());
		else
		    printf("Id %d used but not found\n", i);
	    }
	}
    return TCL_OK;
}
#endif

void TclMusInit(Tcl_Interp *interp)
{
    Tcl_InitHashTable(&dstable, TCL_STRING_KEYS);
    Tcl_CreateCommand(interp, "makesound", DSSoundCmd, (ClientData*)NULL,
			(Tcl_CmdDeleteProc*)NULL);
    Tcl_CreateCommand(interp, "makemixer", DSMakeMixerCmd, (ClientData*)NULL,
			(Tcl_CmdDeleteProc*)NULL);
    Tcl_CreateCommand(interp, "setinput", DSSetInputCmd, (ClientData*)NULL,
			(Tcl_CmdDeleteProc*)NULL);
    Tcl_CreateCommand(interp, "setvolume", DSSetVolumeCmd, (ClientData*)NULL,
			(Tcl_CmdDeleteProc*)NULL);
    Tcl_CreateCommand(interp, "makesequence", DSMakeSeqCmd, (ClientData*)NULL,
			(Tcl_CmdDeleteProc*)NULL);
    Tcl_CreateCommand(interp, "addsource", DSAddSourceCmd, (ClientData*)NULL,
			(Tcl_CmdDeleteProc*)NULL);
    Tcl_CreateCommand(interp, "makefilter", DSFilterCmd, (ClientData*)NULL,
			(Tcl_CmdDeleteProc*)NULL);
    Tcl_CreateCommand(interp, "modulate", DSModulateCmd, (ClientData*)NULL,
			(Tcl_CmdDeleteProc*)NULL);
    Tcl_CreateCommand(interp, "clonesound", DSCloneCmd, (ClientData*)NULL,
			(Tcl_CmdDeleteProc*)NULL);
    Tcl_CreateCommand(interp, "destroysound", DSDestroyCmd, (ClientData*)NULL,
			(Tcl_CmdDeleteProc*)NULL);
    Tcl_CreateCommand(interp, "playsound", DSPlayCmd, (ClientData*)NULL,
			(Tcl_CmdDeleteProc*)NULL);
    Tcl_CreateCommand(interp, "plotsound", DSPlotCmd, (ClientData*)NULL,
			(Tcl_CmdDeleteProc*)NULL);
    Tcl_CreateCommand(interp, "expandsound", DSExpandCmd, (ClientData*)NULL,
			(Tcl_CmdDeleteProc*)NULL);
    Tcl_CreateCommand(interp, "savesound", DSSaveCmd, (ClientData*)NULL,
			(Tcl_CmdDeleteProc*)NULL);
    Tcl_CreateCommand(interp, "soundlength", DSLengthCmd, (ClientData*)NULL,
			(Tcl_CmdDeleteProc*)NULL);
    Tcl_CreateCommand(interp, "soundrate", DSRateCmd, (ClientData*)NULL,
			(Tcl_CmdDeleteProc*)NULL);
    Tcl_CreateCommand(interp, "soundbits", DSBitsCmd, (ClientData*)NULL,
			(Tcl_CmdDeleteProc*)NULL);
    Tcl_CreateCommand(interp, "playsection", DSPlaySectionCmd, (ClientData*)NULL,
			(Tcl_CmdDeleteProc*)NULL);
#ifdef LEAKCHECK
    Tcl_CreateCommand(interp, "showsounds", DSShowUsedCmd, (ClientData*)NULL,
			(Tcl_CmdDeleteProc*)NULL);
#endif
}

int Tcl_AppInit(Tcl_Interp *interp)
{
    if (Tcl_Init(interp) == TCL_ERROR)
	return TCL_ERROR;
    //tcl_RcFileName = "~/.tclshrc";
    TclMusInit(interp);
    return TCL_OK;
}

int main(int argc, char **argv)
{
    Tcl_Main(argc, argv, Tcl_AppInit);
    return 0;
}



