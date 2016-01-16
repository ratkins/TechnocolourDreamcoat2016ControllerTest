#pragma once

#include <FastLED.h>
#include "Matrix.h"
#include "Effect.h"

#define CONCURRENT_SPARKLES (kNumLeds/20)

struct Sparkle {
    uint8_t x, y;
    uint8_t colourIndex;
};

static CRGB sparklePalette[] = {CRGB::Black, CRGB::White, CRGB::DarkBlue};

class Scintillate : public Effect {
  
  private:
    bool soundReactive;
    uint8_t brightness;
    Sparkle sparkles[CONCURRENT_SPARKLES];
    
  public:
    Scintillate(CRGB *leds) : Effect(leds), soundReactive(false), brightness(0) {
    }
    
    virtual void draw(Controls controls) {
        for (int i = 0; i < CONCURRENT_SPARKLES; i++) {
            if (sparkles[i].colourIndex == 0) {
                if (random() > 128) {                  
                    sparkles[i].x = random(kMatrixWidth);
                    sparkles[i].y = random(kMatrixHeight);
                    sparkles[i].colourIndex = 1;
                }                
            } else if (sparkles[i].colourIndex == 2) {
                sparkles[i].colourIndex = 0;                
            } else {
                sparkles[i].colourIndex++;
            }
            leds[XY(sparkles[i].x, sparkles[i].y)] = CRGB(sparklePalette[sparkles[i].colourIndex]);
        }
    }
    
};



