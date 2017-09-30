Sound.c

/*

        Kiwi : A Parallel System for Software Sound Synthesis

        William Walker (walker@cs.uiuc.edu)

        Department of Computer Science * University of Illinois
        1304 West Springfield Avenue * Urbana, Illinois 61801-2987

        Sound.c

*/

double fmod(double a, double b) { while (a > b) a -= b; return a; }

samp Sound::nextSample()
{
  report(form("Sound: default nextSample does nothing %x\n", this)); return 0.0;
}

Sound::Sound() { ; }
Sound::~Sound() { ; }

Loudness::Loudness(Sound *a, float b) {
  source = a;
  int notenum = (int) (12.0 * log10(b / (880.0/32.0))/log10(2.0) - 2.6);
  scalar = (90 - notenum) / 70.0;
}

FreqMod::FreqMod(Table *a, Sound *b, float c) {
  table = a; modulator = b; freq = c;
  phase = 0.0;
  tableFreq = table->freq;
  phaseIncrement = freq / tableFreq;
  tableLength = table->length;
}

FreqMod::~FreqMod() { delete(modulator); }

samp FreqMod::nextSample()
{
  phase += ((modulator->nextSample() + 0.5) * phaseIncrement);
  if ((phase + 0.5) > tableLength) phase -= tableLength;
  return table->entry((int) phase);
}

Impulse::Impulse() { flag = 0; }

Impulse::~Impulse() { ; }

samp Impulse::nextSample()
{
  if (flag == 0) { flag = 1; return 0.5; }
  else return 0.0;
}

Noise::Noise() { ; }

Noise::~Noise() { ; }

samp Noise::nextSample()
{
  return (((float) random())/0x7ffffff0) - 0.5;
}

Buzz::Buzz(float a, int b) {
  freq = a; harm = b;
  phase = 0.0;
  sine = new SineTable(10000);
  Assert(sine != NULL);
  sineFreq = sine->freq;
  sineLength = sine->length;
}

Buzz::~Buzz() { ; }

samp Buzz::nextSample()
{

  phase += (freq / sineFreq);
  if ((phase + 0.5) > sineLength) phase -= sineLength;

  if (fabs(sine->entry((int) phase)) < 0.001)
    return 1.0;
  else
    return (1.0 / (2 * harm))
           * sine->entry((int) (fmod((2 * harm + 1) * phase, 1.0 * sineLength)))
           / sine->entry((int) phase)
           - 1;
}

Karplus::Karplus(float a)
{
  freq = a;
  stretch = 0.5;
  loss = 1.0;

  int N = (int) ((SampRate / freq) - stretch - 0.001);
  double x = (SampRate / freq) - N - stretch;
  tune = (1 - x) / (1 + x);

  shift = new RandTable(N + 2);
  Assert(shift != NULL);
  tableLength = shift->length;

  /* initialise string structure */
  filt1 = -tune * (stretch - 1);         /* filter constants */
  filt2 = tune * stretch - stretch + 1;

  p1 = 3; p2 = 2; p3 = 1; p4 = 0;
}

Karplus::~Karplus()
{
  delete(shift);
}

samp Karplus::nextSample()
{
  /* compute present sample and increment pointers for shift reg */
  samp sum = (shift->entry(p1++) * filt1) + (shift->entry(p2++) * filt2) +
             (shift->entry(p3++) * stretch) - (shift->entry(p4++) * tune);

  /* wrap pointers around shift register if necessary */
  p1 %= tableLength; p2 %= tableLength; p3 %= tableLength; p4 %= tableLength;

  /* store new value in shift register */
  shift->store(p4, sum * loss);

  return sum;
}
