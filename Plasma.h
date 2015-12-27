#pragma once

#include <FastLED.h>
#include "Matrix.h"
#include "Effect.h"

class Plasma : public Effect {
  
  private:
    uint16_t frame;
    
  public:
    Plasma(CRGB *leds) : Effect(leds), frame(0) {}
    
    virtual void draw(Controls controls) {
        frame += 100;
        for (int x = 0; x < kMatrixWidth; x++) {
            for (int y = 0; y < kMatrixHeight; y++) {
                int16_t v = 0;
                uint16_t wibble = sin8((frame >> 8) + 128);
                v += sin16(x * wibble * 6 + frame);
                v += cos16(y * (128 - wibble) * 4 + frame);
                v += sin16(y * x * cos8((-frame >> 8) + 128) / 2);
                
                leds[XY(x, y)] = ColorFromPalette(RainbowColors_p, (v >> 8) + 127);
            }
        }
    }
    
};