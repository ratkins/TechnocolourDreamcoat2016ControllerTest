#include <FastLED.h>

#include "Matrix.h"
#include "Effect.h"

//class Plasma : public Effect {
//  
//  private:
//    uint16_t frame;
//    CRGBPalette256 palette;
//    
//  public:
//    Plasma(CRGB *leds) : Effect(leds), frame(0) {}
//    
//    virtual void draw(Controls controls) {
//        frame += 100;
//        for (int x = 0; x < kMatrixWidth; x++) {
//            for (int y = 0; y < kMatrixHeight; y++) {
//                int16_t v = 0;
//                uint16_t wibble = sin8(frame);
//                v += sin16(x * wibble * 8 + frame);
//                v += cos16(y * (128 - wibble) * 4 + frame);
//                v += sin16(y * x * cos8(-frame) / 2);
//                
//                leds[XY(x, y)] = ColorFromPalette(palette, (v >> 8) + 127);
//            }
//        }
//    }
//    
//    uint8_t sin8(uint16_t x) {
//        return (sin16(x) >> 8) + 128;
//    }
//    
//    uint8_t cos8(uint16_t x) {
//        return (cos16(x) >> 8) + 128;
//    }    
//
//};