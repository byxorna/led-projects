#ifndef _EFFECTS_H_
#define _EFFECTS_H_

#ifndef _PARTICLE_H_
#include "Particle.h"
#endif
#ifndef _FASTLED_H_
#include "FastLED.h"
#endif
#ifndef _STRUCTS_H_
#include "structs.h"
#endif

typedef void (*EffectFunction)(Deck*,uint8_t,uint8_t,uint8_t);

void effect_random_decay(Deck* s, uint8_t dw, uint8_t p1, uint8_t p2) {
  for(int i = 0; i < NUM_LEDS; ++i) {
    if (random(NUM_LEDS) < NUM_LEDS/4) {
      s->leds[i].fadeToBlackBy(random(5));
    }
  }
}

void effect_sparkles(Deck* s, uint8_t dw, uint8_t p1, uint8_t p2) {
  for(int i = 0; i < NUM_LEDS; ++i) {
    if (random(NUM_LEDS) == 0) {
      s->leds[i] = NSFastLED::CRGB::White;
    }
  }
}

#endif
