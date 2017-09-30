#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "digsource.h"

class Sequence : public DigitalSource
{
  protected:
    struct SourceNode
    {
	DigitalSource *source;
	int start;
	SourceNode *next;
	SourceNode(DigitalSource *source_in = 0, int start_in = 0)
	    : source(source_in), start(start_in), next(0)
	{}
	~SourceNode()
	{
	    if (source) source->Delete();
	}
    } *head;
    SourceNode *GetNextPlayableNode(SourceNode *n, int p) const;
    virtual int Killed(SourceNode *n, int p) const;
  public:
    Sequence();
    void AddSource(DigitalSource *ds, int start);
    virtual int Data(int p) const;
    virtual int Length();
    virtual char *TypeName() const;
    virtual char Mode() const;
    virtual ~Sequence();
};

class PianoModeSequence : public Sequence
{
  protected:
    virtual int Killed(SourceNode *n, int p) const;
  public:
    PianoModeSequence()
	: Sequence()
    {}
    virtual char Mode() const;
    virtual ~PianoModeSequence();
};

class RetriggerModeSequence : public Sequence
{
  protected:
    virtual int Killed(SourceNode *n, int p) const;
  public:
    RetriggerModeSequence()
	: Sequence()
    {}
    virtual char Mode() const;
    virtual ~RetriggerModeSequence();
};

#endif

