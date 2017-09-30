// Almost everything in the library is a DigitalSource - that is,
// a finite-length source of sampled digital data.
//
// For piano and retrigger mode processing we need to know the
// `instrument' and pitch. This is not a trivial matter, but we 
// adopt a simple approach which should be adequate, namely:
//
// * the `instrument' is the source itself, unless the source
//	is a filter, in which case it is the filter's source's
//	instrument
//
// * the pitch is 0 (octave 2, note 0) unless the source is a
//	filter, in which case it is the filter's source's
//	pitch, except if the filter is a Transpose filter,
//	in which case it is the transposition pitch.
//
// Offset mode processing is more tricky, and is deferred for now.
// It could be implemented using Offset filters.
// 
// As of 14/7/97, we drop the bits and rate from all source and 
// filter classes. A fixed sample rate and bitsize is used for
// all sources and filters instead. Resampling is used on SampleFiles
// to get them to match this, and the Player class can play back
// using different bitsize and rate (also through use of a resampler).
// Apart from these `file' sources and sinks, we need no longer
// be concerned with variations in rates and sample size.

#ifndef DIGSOUND_H
#define DIGSOUND_H

int SampleRate();
int SampleSize();
int SetSampleRate(int v);
int SetSampleSize(int v);

// uncomment this for memory leak checking and the showsounds command.

#define LEAKCHECK

// The mother of all classes

class DigitalSource
{
  protected:
#ifdef LEAKCHECK
    const int max_sources = 20000;
    static unsigned long usevec[(max_sources+31)/32];
#endif
    const int UnknownLength = -1;
    static int nextid;
    int length;		// length, set with lazy evaluation
    int pos;		// used for iterating through a source; discouraged
    int id;		// unique instance id
    int refcnt;		// reference count for memory management

  public:

    DigitalSource(int length_in = UnknownLength);
    inline int ID() const
    {
	return id;
    }
#ifdef LEAKCHECK
    static int MaxSources();
    static int InUse(int id);
#endif
    virtual const DigitalSource *Instrument() const;
    virtual int Pitch() const;
    virtual int Length();
    virtual int Data(int p) const;
    inline int NextSample()
    {
	return Data(pos++);
    }
    inline void Reset()
    {
	pos = 0;
    }
    // Crude RTTI
    virtual char *TypeName() const;
    virtual char *Name() const;
    // Rather than smart pointers, we use a crude, explicit, reference
    // counting scheme
    virtual DigitalSource *Clone();
    virtual int Delete();
    void WritePlotData(char *fname, int len = -1);
    void XPlot(int length = -1);
    virtual ~DigitalSource();
};

//----------------------------------------------------------

#endif

