class Loudness : public Sound {
  Sound *       source;
  float         pitch;
  float         scalar;

public:
  samp          nextSample() { return source->nextSample() * scalar; }
  Loudness(Sound*, float);
  ~Loudness() { delete(source); }
};

class Broken : public Sound {
  int           counter;
  int           flag;
  Sound *       source;

public:
  samp          nextSample();
  Broken(Sound*);
 ~Broken();
};

class RingMod : public Sound {
  Sound *       A;
  Sound *       B;

public:
  samp          nextSample() { return A->nextSample() * B->nextSample(); }
  RingMod(Sound *a, Sound *b) {A = a; B = b;}
 ~RingMod() {delete(A); delete(B);}
};

class FreqMod : public Sound {
  Sound *       modulator;
  Table *       table;
  float         tableFreq;
  float         phase;
  float         phaseIncrement;
  float         freq;
  int           tableLength;

public:
  samp          nextSample();
  FreqMod(Table*, Sound*, float);
 ~FreqMod();
};

class Mixer : public Sound {
  Sound *       sources[512];
  float         weights[512];
  int           howMany;
  float         maximumAmplitude;

public:
  void          addSource(Sound*, float);
  samp          nextSample();
  Mixer();
 ~Mixer();
};

class EnvGen : public Sound {

protected:
  struct { float y; int x; } breakpoint[32];
  Sound *       source;
  int           count;
  int           which;
  int           howMany;
  float         currentValue, factor;

public:
  void          addBreak(float, float);
  virtual samp  nextSample() = 0;
  EnvGen(Sound *a);
 ~EnvGen();
};

class LinGen : public EnvGen {

public:
  samp nextSample();
  LinGen(Sound *a) : (a) {};
};

class ExpGen : public EnvGen {

public:
  samp nextSample();
  ExpGen(Sound *a) : (a) {};
};

class Filter : public Sound {

protected:
  float         centerFreq, damp;
  samp          state0, state1, state2, state3;
  samp          parm0, parm1, parm2, parm3, parm4;
  samp          parm5, parm6, parm7, parm8, parm9;
  Sound *       source;
  void          setParameters();

public:
  virtual samp  nextSample() = 0;
  Filter(Sound*, float, float);
 ~Filter();
};

class LowPass : public Filter {

public:
  samp nextSample();
  LowPass(Sound *a, float b, float c) : (a, b, c) {};
};

class HighPass : public Filter {

public:
  samp nextSample();
  HighPass(Sound *a, float b, float c) : (a, b, c) {};
};

class LowPass2 : public Filter {

public:
  samp nextSample();
  LowPass2(Sound *a, float b, float c) : (a, b, c) {};
};

class HighPass2 : public Filter {

public:
  samp nextSample();
  HighPass2(Sound *a, float b, float c) : (a, b, c) {};
};

class BandPass : public Filter {

public:
  samp nextSample();
  BandPass(Sound *a, float b, float c) : (a, b, c) {};
};

class BandStop : public Filter {

public:
  samp nextSample();
  BandStop(Sound *a, float b, float c) : (a, b, c) {};
};

class DelayFilter : public Filter {

protected:
  Table *       buffer;
  int           index;

public:
  virtual samp  nextSample() = 0;
  DelayFilter(Sound *a, float b, int c) : (a, 0, 0)
    { index = 0; parm0 = b; buffer = new BlankTable(c); }
 ~DelayFilter() { delete(buffer); }
};

class AllPass : public DelayFilter {

public:
  samp nextSample();
  AllPass(Sound *a, float b, int c) : (a, b, c) {;}
};

class Comb : public DelayFilter {

public:
  samp nextSample();
  Comb(Sound *a, float b, int c) : (a, b, c) {;}
};

class Flanger : public DelayFilter {
  float         phase1, phaseIncrement1;
  float         phase2, phaseIncrement2;

public:
  samp          nextSample();
  Flanger(Sound*, float, int);
};
