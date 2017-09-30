#ifndef SAMPLEF_H
#define SAMPLEF_H

#include <stdio.h>
#include "vector.h"

class SampleFile : public Vector
{
  protected:
    char *name;
    void MakeSigned();
    virtual char *LoadHeader(FILE *fp) = 0;
    virtual char *LoadData(FILE *fp);
    virtual char *Save(FILE *fp);
  public:
    SampleFile(const char *sname, int bits_in = 0, int rate_in = 0);
    virtual int Length();
    virtual int Data(int p) const;
    void SetName(const char *name_in);
    virtual char *Name() const;
    char *Load();
    char *Save();
    inline int Bits() const
    {
	return bits;
    }
    inline int Rate() const
    {
	return rate;
    }
    virtual char *TypeName() const;
    virtual ~SampleFile();
};

//----------------------------------------------------------------

class Raw8BitSample : public SampleFile
{
  protected:
    virtual char *LoadHeader(FILE *fp);
    virtual char *LoadData(FILE *fp);
    virtual char *Save(FILE *fp);
  public:
    Raw8BitSample(const char *fname_in, int rate_in)
      : SampleFile(fname_in, 8, rate_in)
    {}
    virtual char *TypeName() const;
    virtual ~Raw8BitSample();
};

//----------------------------------------------------------------

class WAVSample : public SampleFile
{
  protected:
    struct wave_header // header for WAV-Files
    {
	char		main_chunk[4];	// 'RIFF'
	unsigned long	length;		// length of file
	char		chunk_type[4];	// 'WAVE'
	char		sub_chunk[4];	// 'fmt'
	unsigned long	length_chunk;	// length sub_chunk, always 16 bytes
	unsigned short	format;		// always 1 = PCM-Code
	unsigned short	modus;		// 1 = Mono, 2 = Stereo
	unsigned long	sample_fq;	// Sample Freq
	unsigned long	byte_p_sec;	// Data per sec
	unsigned short	byte_p_spl;	// bytes per sample
					//     1=8 bit, 2=16 bit (mono)
					//     2=8 bit, 4=16 bit (stereo)
	unsigned short	bit_p_spl;	// bits per sample, 8, 12, 16
	char		data_chunk[4];	// 'data'
	unsigned long	data_length;	// length of data
    } whdr;

    char *CheckHeader(wave_header &head, int fd);
    virtual char *LoadHeader(FILE *fp);
    virtual char *LoadData(FILE *fp);
    virtual char *Save(FILE *fp);
  public:
    WAVSample(const char *fname_in = 0)
      : SampleFile(fname_in)
    {}
    virtual char *TypeName() const;
    virtual ~WAVSample();
};

//----------------------------------------------------------------

class SampleBank
{
  protected:
    const int maxsamples = 256;
    DigitalSource **sample;
    char **name;
    int numsamples;
    static SampleBank *instance;

    SampleBank();
    DigitalSource *Find(const char *sname);
    DigitalSource *Find(int id);
    DigitalSource *Add(const char *sname, int bits, int rate);
  public:
    static DigitalSource *Get(const char *sname, int bits_in=0, int rate_in=0);
    static DigitalSource *Get(int id);
    ~SampleBank();
};

#endif

