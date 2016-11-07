
class LedInvertedPinLevelFeature
{
  public:
  static const uint8_t LedOff = HIGH;
  static const uint8_t LedOn = LOW;
};

class LedNormalPinLevelFeature
{
public:
  static const uint8_t LedOff = LOW;
  static const uint8_t LedOn = HIGH;
};

template<typename T_PINLEVEL_FEATURE> class StatusLed
{
public:
  StatusLed(uint8_t pin) :
      _pin(pin),
      _beat(0),
      _msBeat(0)
  {
  }

  void begin()
  {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, T_PINLEVEL_FEATURE::LedOff); 
  }
  
  void setBlinkBeat(uint16_t beat, uint16_t msBeat)
  {
    if (_beat != beat)
    {
      digitalWrite(_pin, T_PINLEVEL_FEATURE::LedOff); 
      _beat = beat;
      _msBeat = msBeat;     
      _tick = 0x8000; // left most bit
      _lastTime = millis(); 
    }
  }

  void stop()
  {
    _beat = 0;
    _msBeat = 0;
    digitalWrite(_pin, T_PINLEVEL_FEATURE::LedOff); 
  }

  void loop()
  {
    if (_beat)
    {
      uint32_t time = millis();
      uint32_t delta = time - _lastTime;
      if (delta >= _msBeat)
      {
        // trigger a beat
        digitalWrite(_pin, (_beat & _tick) ? 
            T_PINLEVEL_FEATURE::LedOn : T_PINLEVEL_FEATURE::LedOff); 
        
        _tick = (_tick >> 1) | (_tick << 15); // rotate right
                                
        // roll over left over time
        _lastTime = time + (delta -_msBeat);
      }
    }
  }
  
private:
  const uint8_t _pin;

  uint32_t _lastTime;
  uint16_t _beat; // beat pattern in binary
  uint16_t _msBeat;  //  milliseconds per beat bit
  uint16_t _tick; // beat tracking in binary
};

