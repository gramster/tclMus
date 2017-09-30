#include "resampler.h"
#include "sequence.h"

// NOTE: We insert sources in order of starting time.

Sequence::Sequence()
    : DigitalSource(),
      head(new SourceNode(0, 0)) // dummy head node
{
}

void Sequence::AddSource(DigitalSource *ds, int start)
{
    if (ds == 0) return;
    SourceNode *n = new SourceNode(ds, start);
    int l = start + ds->Length();
    if (l > length)
    {
	length = l;
    }
    SourceNode *tmp = head->next, *prev = head;
    while (tmp)
    {
	if (start <= tmp->start) break;
	prev = tmp;
	tmp = tmp->next;
    }
    prev->next = n;
    n->next = tmp;
}

Sequence::SourceNode *Sequence::GetNextPlayableNode(SourceNode *sn, int p) const
{
    while (sn && p >= sn->start)
    {
	if (p < (sn->start+sn->source->Length()))
	    return sn;
	sn = sn->next;
    }
    return 0;
}

int Sequence::Killed(SourceNode *n, int p) const
{
    (void)n; (void)p;
    return 0;
}

int Sequence::Data(int p) const
{
    int v = 0;
    SourceNode *tmp = GetNextPlayableNode(head->next, p);
    while (tmp)
    {
	if (!Killed(tmp, p))
	    v += tmp->source->Data(p-tmp->start);
	tmp = GetNextPlayableNode(tmp->next, p);
    }
    return v;
}

int Sequence::Length()
{
    if (length < 0)
    {
        SourceNode *tmp = head->next;
	length = 0;
	while (tmp)
	{
	    int l = tmp->start + tmp->source->Length();
	    if (l > length) length = l;
	    tmp = tmp->next;
	}
    }
    return length;
}

char *Sequence::TypeName() const
{
    return "Sequence";
}

char Sequence::Mode() const
{
    return 'M';
}

Sequence::~Sequence()
{
    while (head)
    {
	SourceNode *tmp = head;
	head = head->next;
	delete tmp;
    }
}

//------------

char PianoModeSequence::Mode() const
{
    return 'P';
}

int PianoModeSequence::Killed(SourceNode *n, int p) const
{
    int pitch = n->source->Pitch();
    const DigitalSource *inst = n->source->Instrument();
    SourceNode *tmp = GetNextPlayableNode(n->next, p);
    while (tmp)
    {
	if (tmp->source->Pitch() == pitch && tmp->source->Instrument() == inst)
	    return 1;
	tmp = GetNextPlayableNode(tmp->next, p);
    }
    return 0;
}

PianoModeSequence::~PianoModeSequence()
{
}

//------------

char RetriggerModeSequence::Mode() const
{
    return 'R';
}

int RetriggerModeSequence::Killed(SourceNode *n, int p) const
{
    const DigitalSource *inst = n->source->Instrument();
    SourceNode *tmp = GetNextPlayableNode(n->next, p);
    while (tmp)
    {
	if (tmp->source->Instrument() == inst)
	    return 1;
	tmp = GetNextPlayableNode(tmp->next, p);
    }
    return 0;
}

RetriggerModeSequence::~RetriggerModeSequence()
{
}

