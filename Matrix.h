#ifndef Matrix_h
#define Matrix_h

//
// Matrix.h
//
// Lookup table and coresponding XY() function for the Technocolour Dreamcoat.
// Technique cribbed from https://github.com/macetech/RGBShades/blob/master/XYmap.h
//

const uint8_t kMatrixWidth = 36;
const uint8_t kMatrixHeight = 20;
const uint16_t kNumLeds = kMatrixWidth * kMatrixHeight;

extern const uint16_t matrix[kNumLeds];

static inline uint16_t XY(uint8_t x, uint8_t y) { return matrix[(x * kMatrixHeight) + y]; }

#endif
