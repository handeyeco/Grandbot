class Voice {
  private:
    int m_voicePin;
    static const int pitches[];
  public:
    Voice(int voicePin);
    void happy();
    void sleepy();
    void playRandomSequence();
    void playRandomNote();
    void play(int note);
    void play(int note, int duration);
};