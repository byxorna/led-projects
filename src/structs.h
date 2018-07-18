#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#ifndef _PARTICLE_H_
#include "Particle.h"
#endif
#ifndef _FASTLED_H_
#include "FastLED.h"
#endif

typedef struct Deck {
  uint8_t label;
  float crossfadePositionActive;
  uint8_t pattern;
  uint8_t palette;
  uint8_t animationIndex;
  NSFastLED::CRGBPalette16 currentPalette; // current color palette
  unsigned long tPatternStart;  // time last pattern changed
  unsigned long tPaletteStart;  // time last palette changed
  NSFastLED::CRGB* leds;
} Deck;

typedef struct Output {
  NSFastLED::CRGB* leds;
} Output;

typedef struct Mixer {
  float crossfadePosition;
  int crossfadeDirection;
  uint8_t crossfadeInProgress;
  unsigned long tLastCrossfade;
  Deck* a;
  Deck* b;
  Output* out;
} Mixer;

#endif
