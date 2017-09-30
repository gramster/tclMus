#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include "resampler.h"
#include "samplef.h"

SampleFile::SampleFile(const char *name_in, int bits_in, int rate_in)
    : Vector(0, bits_in, rate_in), name(0)
{
    SetName(name_in);
}

int SampleFile::Length()
{
    return Vector::Length();
}

int SampleFile::Data(int p) const
{
    return Vector::Data(p);
}

char *SampleFile::TypeName() const
{
    return "SampleFile";
}

char *SampleFile::Name() const
{
    return name;
}

void SampleFile::SetName(const char *name_in)
{
    delete [] name;
    if (name_in)
    {
    	name = new char[strlen(name_in)+1];
    	if (name) strcpy(name, name_in);
    }
    else name = 0;
}

void SampleFile::MakeSigned()
{
    // make signed
    if (bits == 8)
	for (unsigned i = 0; i < nbytes; i++)
	    data[i] -= 128;
    else // 16-bit; untested
	for (unsigned i = 0; i < nbytes; i+=2)
	{
	    unsigned short v = (data[i+1]<<8) | data[i];
	    v -= 32768;
	    data[i+1] = (v>>8); data[i] = v & 0xFF;
	}
}

char *SampleFile::LoadData(FILE *fp)
{
    int n = fread(data, 1, nbytes, fp);
    if (n != (int)nbytes)
    {
	fprintf(stderr, "Short read: expected %d, got %d\n", nbytes, n);
	return "Read failed";
    }
    return 0;
}

char *SampleFile::Load()
{
    char *rtn = 0;
    delete [] data;
    FILE *fp = fopen(name, "r");
    if (fp == 0)
	rtn = "Couldn't open sample file";
    else 
    {
	rtn = LoadHeader(fp);
	if (rtn == 0)
	{
    	    if (nbytes == 0)
		data = 0;
    	    else 
    	    {
		data = new unsigned char[nbytes];
		if (data == 0)
		    rtn = "Memory allocation failed";
		else
		{
		    rtn = LoadData(fp);
		    if (rate == 0) rate = 8000; // sanity
		}
    	    }
	}
    	fclose(fp);
    }
    return rtn;
}

char *SampleFile::Save(FILE *fp)
{
    if (fwrite(data, 1, nbytes, fp) != nbytes)
	return "Write failed";
    return 0;
}

char *SampleFile::Save()
{
    char *rtn = 0;
    FILE *fp = fopen(name, "w");
    if (fp == 0)
	rtn = "Couldn't open sample file";
    else
    {
	rtn = Save(fp);
	fclose(fp);
    }
    return rtn;
}

SampleFile::~SampleFile()
{
}

//-----------------------------------------------------------------

char *Raw8BitSample::TypeName() const
{
    return "Raw8BitSample";
}

char *Raw8BitSample::LoadHeader(FILE *fp)
{
    if (fseek(fp, 0, SEEK_END) < 0)
	return "Couldn't fseek";
    nbytes = ftell(fp);
    rewind(fp);
    return 0;
}

char *Raw8BitSample::LoadData(FILE *fp)
{
    char *rtn = SampleFile::LoadData(fp);
    if (rtn) return rtn;
#if 1
    if (strcmp(name+strlen(name)-4, ".ami") != 0)
	MakeSigned();
#endif
    length = nbytes;
    return 0;
}

char *Raw8BitSample::Save(FILE *fp)
{
    return SampleFile::Save(fp);
}

Raw8BitSample::~Raw8BitSample()
{
}

//-------------------------------------------------------------------

char *WAVSample::TypeName() const
{
    return "WAVSample";
}

char *WAVSample::CheckHeader(wave_header &head, int fd)
{
    if (strncmp(head.main_chunk,"RIFF",4))
	return "Not a RIFF file";
    else if (strncmp(head.chunk_type,"WAVE",4))
	return "Not a WAV file";
    /* Scan for fmt-chunk, then re-read it */
    if (strncmp(head.sub_chunk, "fmt ", 4))
    {
	lseek(fd, 0, SEEK_SET);
	while(strncmp(head.sub_chunk, "fmt ", 4))
	{
	    lseek(fd, -2, SEEK_CUR);		/* Catch off-by-two errors */
	    if (read(fd, &head.sub_chunk, 4) != 4)
		return "Error scanning file for fmt-chunk!";
	    if (read(fd, &head.length_chunk, 4) != 4)
		return "Found fmt-chunk, but couldn't read length field!";
	    int n = sizeof(wave_header) -
			(((char*)&head.format) - ((char*)&head));
	    if (read(fd, &head.format, n) != n)
		return "Reading fmt-chunk failed!";
	}
    }

    // Do we really have a data-chunk?

    if (strncmp(head.data_chunk, "data", 4))
    {
	/* Ah, there wasn't any data-chunk! Well, search for it then! (Of
	 * course, we could have done this much quicker, by calculating where
	 * the data-chunk would be most likely to begin, but then we would have
	 * to read more data into the header-buffer etc. etc... Nah, we'll do
	 * it the simple and dirty way!
	 */
	lseek(fd, -2, SEEK_CUR); // Go back 2 bytes, so we won't miss anything.
	while (strncmp(head.data_chunk, "data", 4))
	{
	    lseek(fd, -2, SEEK_CUR);	/* Catch off-by-two errors */
	    if (read(fd, &head.data_chunk, 4) != 4)
		return "Error scanning file for data-chunk!";
	}
	/* So far, so good. Now try to read the length of the data-chunk! */
	if (read(fd, &head.data_length, 4) != 4)
	    return "Found data-chunk, but couldn't read length field!";
    }

    /* One final check, this time to ensure that the data_length is within sane
     * limits, i.e. the length of the file... */

    if (head.data_length >= head.length)
    {
	// truncate it
	head.data_length = head.length - lseek(fd, 0, SEEK_CUR);
    }

    if ( strncmp(head.sub_chunk,"fmt ",4) )
	return "Not a WAV file";

    if ((head.sample_fq * head.byte_p_spl) != head.byte_p_sec) 
	return "Invalid sampling rate";
    return 0;
}

char *WAVSample::LoadHeader(FILE *fp)
{
    int n = read(fileno(fp), &whdr, sizeof(whdr));
    if (n < (int)sizeof(whdr))
	return "Couldn't read header";
    char *errmsg = CheckHeader(whdr, fileno(fp));
    if (errmsg) return errmsg;

    rate = whdr.sample_fq;
    length = nbytes = whdr.data_length;

    if (whdr.modus == 2) // stereo; not handled properly yet
	length >>= 1;
    bits = whdr.bit_p_spl;
    if (bits != 8) // 12 or 16-bit
	length >>= 1;
printf("Rate %d length %d nbytes %d mode %d bits %d\n",
		rate, length, nbytes, whdr.modus, whdr.bit_p_spl);
    return 0;
}

char *WAVSample::LoadData(FILE *fp)
{
    char *rtn = SampleFile::LoadData(fp);
    //if (rtn == 0) MakeSigned();
    return rtn;
}

char *WAVSample::Save(FILE *fp)
{
    (void)fp;
    return 0;
}

WAVSample::~WAVSample()
{
}

//-------------------------------------------------------------------

SampleBank *SampleBank::instance = 0;

SampleBank::SampleBank()
    : numsamples(0)
{
    sample = new DigitalSource*[maxsamples];
    name = new char*[maxsamples];
    for (int i = 0; i < maxsamples; i++)
    {
	sample[i] = 0;
	name[i] = 0;
    }
}

DigitalSource *SampleBank::Find(const char *sname)
{
    for (int i = 0; i < numsamples; i++)
 	if (strcmp(name[i], sname) == 0)
	    return sample[i];
    return 0;
}

DigitalSource *SampleBank::Find(int id)
{
    for (int i = 0; i < numsamples; i++)
 	if (sample[i]->ID() == id)
	    return sample[i];
    return 0;
}

DigitalSource *SampleBank::Add(const char *sname, int bits, int rate)
{
    if (numsamples < maxsamples)
    {
	SampleFile *f;
	int nl = strlen(sname);
	if (strncasecmp(sname+nl-4, ".wav", 4) == 0)
	    f  = new WAVSample(sname);
	else
	    f  = new Raw8BitSample(sname, rate);
	char *errmsg = ((SampleFile *)f)->Load();
	DigitalSource *s = f;
	if (errmsg == 0)
	{
	    printf("Added %s (%d/%d/%d) to bank\n",
			sname, f->Bits(), f->Rate(), f->Length());
	    if (f->Bits() != SampleSize() || f->Rate() != SampleRate())
	    {
		printf("resampling at %d/%d\n", SampleSize(), SampleRate());
		s = new Resampler(f, f->Bits(), f->Rate(),
					SampleSize(), SampleRate());
	    }
	    sample[numsamples] = s;
	    name[numsamples] = new char[strlen(sname)+1];
	    strcpy(name[numsamples], sname);
	    numsamples++;
	}
	else
	{
	    fprintf(stderr, "Failed to add %s to samplebank: %s\n",
			sname, errmsg);
	    delete f;
	    s = f = 0;
	}
	return s;
    }
    return 0;
}

DigitalSource *SampleBank::Get(const char *sname, int bits, int rate)
{
    if (instance == 0) instance = new SampleBank();
    DigitalSource *f = instance->Find(sname);
    if (f == 0) f = instance->Add(sname, bits, rate);
    return f;
}

DigitalSource *SampleBank::Get(int id)
{
    if (instance == 0) instance = new SampleBank();
    return instance->Find(id);
}

SampleBank::~SampleBank()
{
    for (int i = 0; i < numsamples; i++)
    {
	delete sample[i];
	delete [] name[i];
    }
    delete [] sample;
    delete [] name;
}

