/*
* Project d18-scarf
* Description: LEDs for DI8ORIENT scarf
* Author: Gabe Conradi
* Date: idklol
*/

#pragma once
#include "Particle.h"
#include "FastLED.h"

struct Deck {
  uint8_t label;
  float crossfadePositionActive;
  uint8_t pattern;
  uint8_t palette;
  uint8_t animationIndex;
  NSFastLED::CRGBPalette16 currentPalette; // current color palette
  unsigned long tPatternStart;  // time last pattern changed
  unsigned long tPaletteStart;  // time last palette changed
  NSFastLED::CRGB* leds;
};

struct Output {
  NSFastLED::CRGB* leds;
};

typedef void (*DrawFunction)(Deck*);
typedef void (*EffectFunction)(Deck*);
// how 2 decks mix together into an output
typedef void (*MixerFunction)(Deck*, Deck*, Output*);

struct Mixer {
  float crossfadePosition;
  int crossfadeDirection;
  uint8_t crossfadeInProgress;
  unsigned long tLastCrossfade;
  Deck* a;
  Deck* b;
  Output* out;
};

