#include <FastLED.h>
#include <Bounce2.h>

#define NUM_LEDS 128
#define LED_PIN 0
const uint8_t kMatrixWidth = 16;
const uint8_t kMatrixHeight = 8;
CRGB leds[NUM_LEDS];
const bool kMatrixSerpentineLayout = true;

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
  
  pinMode(LED_PIN, OUTPUT);

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
  
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(128);
  
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
  
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  for (int y = 0; y < spectrumBands; y++) {
    uint8_t vol = map(spectrumValue[y], 0, 1023, 0, 15);
    leds[XY(vol, y)] = CRGB(255, 255, 255);  
  }

  if (controlButton) {
    fill_solid(leds, NUM_LEDS, CRGB::Blue);
  }
  if (upButton) {
    fill_solid(leds, NUM_LEDS, CRGB::Green);
  }
  if (downButton) {
    fill_solid(leds, NUM_LEDS, CRGB::Red);
  }

  register_marks();    
  
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

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  
  if( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if( kMatrixSerpentineLayout == true) {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }
  
  return i;
}

void register_marks() {
  leds[XY(0, 0)] = CRGB::Red;
  leds[XY(kMatrixWidth - 1, 0)] = CRGB::Green;
  leds[XY(kMatrixWidth - 1, kMatrixHeight - 1)] = CRGB::Blue;
  leds[XY(0, kMatrixHeight - 1)] = CRGB::White;  
}

