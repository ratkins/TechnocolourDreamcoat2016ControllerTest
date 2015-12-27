#include <FastLED.h>
#include <Bounce2.h>

#include "Matrix.h"

const uint8_t ledPin = 0;
const uint8_t maxPowerLedPin = 13;
CRGB leds[kNumLeds];

const int analogPin = 14; // read from multiplexer using analog input 0
const int strobePin = 15; // strobe is attached to digital pin 2
const int resetPin = 12; // reset is attached to digital pin 3
const int spectrumBands = 7;
int spectrumValue[spectrumBands]; // to hold a2d values

// Master brightness pot
const int brightnessPotPin = 16;

// Control (blue) button
const int controlButtonPin = 18;
Bounce controlBounce = Bounce();
bool controlButton = false;

// Down (red) button
const int downButtonPin = 19;
Bounce downBounce = Bounce();
bool downButton = false;

// Up (green) button
const int upButtonPin = 20;
Bounce upBounce = Bounce();
bool upButton = false;

void setup() {
  Serial.begin(9600);

  delay(2000);
  
  pinMode(brightnessPotPin, INPUT);
  
  pinMode(controlButtonPin, INPUT_PULLUP);
  controlBounce.attach(controlButtonPin);
  controlBounce.interval(5);
  
  pinMode(downButtonPin, INPUT_PULLUP);
  downBounce.attach(downButtonPin);
  downBounce.interval(5);
  
  pinMode(upButtonPin, INPUT_PULLUP);
  upBounce.attach(upButtonPin);
  upBounce.interval(5);
  
  FastLED.addLeds<WS2812B, ledPin, GRB>(leds, kNumLeds);
//  FastLED.setBrightness(32);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 2000);
  pinMode(ledPin, OUTPUT);
  set_max_power_indicator_LED(maxPowerLedPin);
  
  pinMode(maxPowerLedPin, OUTPUT);
    
  pinMode(analogPin, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  analogReference(EXTERNAL);
  
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
}

void loop() {

  updateMasterBrightness();
  updateButtonValues();
  updateSpectrumValues();

// simple chase test pattern
//  fill_solid(leds, kNumLeds, CRGB::Black);
//  static uint16_t i = 0;
//  leds[i++] = CRGB::White;
//  if (i == kNumLeds) {
//    i = 0;
//  }

// Line rising up from bottom to top
  fill_solid(leds, kNumLeds, CRGB::Black);
  static uint16_t y = 0;

  for (int x = 0; x < kMatrixWidth; x++) {
    leds[XY(x, y)] = CRGB::White;
  }
  if (++y == kMatrixHeight) {
    y = 0;
  }

// Basic MSEQ7 output
//  fadeToBlackBy(leds, kNumLeds, 16);
//
//  for (int y = 0; y < spectrumBands; y++) {
//    uint8_t vol = map(spectrumValue[y], 0, 1023, 0, 15);
//    for (int x = 0; x < vol; x++) {
//      leds[XY(vol, y)] = CRGB(255, 255, 255);
//    }
//  }

  if (controlButton) {
    fill_solid(leds, kNumLeds, CRGB::Blue);
  }
  if (upButton) {
    fill_solid(leds, kNumLeds, CRGB::Green);
  }
  if (downButton) {
    fill_solid(leds, kNumLeds, CRGB::Red);
  }

//  register_marks();    
  
  Serial.println();
  FastLED.show();
}

void updateSpectrumValues() {
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);  
  for (int i = 0; i < 7; i++) {
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30); // to allow the output to settle
    spectrumValue[i] = analogRead(analogPin);
    digitalWrite(strobePin, HIGH);    
  }
// comment out/remove the serial stuff to go faster
// - its just here for show
//    if (spectrumValue[i] < 10) {
//      Serial.print(" ");
//      Serial.print(spectrumValue[i]);
//    } else if (spectrumValue[i] < 100 ) {
//      Serial.print(" ");
//      Serial.print(spectrumValue[i]);
//    } else {
//      Serial.print(" ");
//      Serial.print(spectrumValue[i]);
//    }
}

void updateButtonValues() {
  controlBounce.update();
  controlButton = controlBounce.read() == LOW;
  
  upBounce.update();
  upButton = upBounce.read() == LOW;
  
  downBounce.update();
  downButton = downBounce.read() == LOW;

  Serial.print("control button = "); Serial.println(controlButton);
  Serial.print("     up button = "); Serial.println(upButton);
  Serial.print("   down button = "); Serial.println(downButton);
}

void updateMasterBrightness() {
  uint8_t masterBrightness = map(analogRead(brightnessPotPin), 0, 1023, 0, 255);
  FastLED.setBrightness(masterBrightness);
  Serial.print("brightness pot = "); Serial.println(masterBrightness);  
}

void register_marks() {
  leds[XY(0, 0)] = CRGB::Red;
  leds[XY(kMatrixWidth - 1, 0)] = CRGB::Green;
  leds[XY(kMatrixWidth - 1, kMatrixHeight - 1)] = CRGB::Blue;
  leds[XY(0, kMatrixHeight - 1)] = CRGB::White;  
}

