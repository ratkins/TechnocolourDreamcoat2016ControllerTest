#include <FastLED.h>
#include "Matrix.h"
#include "Effect.cpp"

class Chase : public Effect {
  
  private:
    uint16_t frame;
    
  public:
    Chase(CRGB *leds) : Effect(leds), frame(0) {}
    
    virtual void draw(Controls controls) {
        leds[frame++ % kNumLeds] = CRGB::White;
    }
    
};
