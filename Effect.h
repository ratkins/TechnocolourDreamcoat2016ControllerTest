#pragma once

//
// Base class for effect instances
//

#include "FastLED.h"

struct Controls {
    
    static const uint8_t spectrumBandsCount = 7;
    uint16_t spectrumBands[spectrumBandsCount];
    
    bool button = false;
    
};

class Effect {
  
  protected:
    CRGB *leds;
    
  public:
    Effect(CRGB *leds) : leds(leds) {}
    
    virtual void draw(Controls controls) = 0;
    
};