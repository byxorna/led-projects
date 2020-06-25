/* Project: home-lasik-accent-leds-orange-purple.ino
* More Info: https://www.youtube.com/watch?v=L3_vNGUauho
*/
#include "FastLED.h"
#include "palettes.h"
#include "structs.h"


DeckSettings deckSettingsA;
DeckSettings deckSettingsB;
DeckSettings* deckSettingsAll[] = {&deckSettingsA, &deckSettingsB};

typedef void (*FP)(CRGB*, DeckSettings*);

// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b)  ((x>b)?b:0)                              // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0


#define NUM_LEDS 130 // anything more than 130~ crashes arduino trinket pros
#define LEDS_PIN 3
#define LED_TYPE NEOPIXEL
#define UPDATES_PER_SECOND 60
#define MAX_SATURATION 255
#define BOOTUP_ANIM_DURATION_MS 5000
#define PATTERN_CHANGE_INTERVAL_MS 30000
#define PALETTE_CHANGE_INTERVAL_MS 30000
#define AUTO_CHANGE_PALETTE 1
#define AUTO_PATTERN_CHANGE true
#define GLOBAL_BRIGHTNESS 170

#define VJ_CROSSFADING_ENABLED 1
// make the crossfade take a long time to mix patterns up a lot and get interesting behavior
#define VJ_CROSSFADE_DURATION_MS 10000.0
#define VJ_NUM_DECKS 2
#define VJ_DECK_SWITCH_INTERVAL_MS 15000

/* crossfading global state */
CRGB masterOutput[NUM_LEDS];
CRGB deckA[NUM_LEDS];
CRGB deckB[NUM_LEDS];
float crossfadePosition = 1.0;  // 0.0 is deckA, 1.0 is deckB
int crossfadeDirection = (crossfadePosition == 1.0) ? -1 : 1; // start going B -> A
uint8_t crossfadeInProgress = 0;
unsigned long tLastCrossfade = 0;

unsigned long t_now;                // time now in each loop iteration
unsigned long t_boot;               // time at bootup
TBlendType currentBlending = LINEARBLEND;



#include "effects.h"

void randomPattern(DeckSettings* deck, DeckSettings* otherDeck) {
  uint8_t old = deck->pattern;
  while (deck->pattern == old || deck->pattern == otherDeck->pattern) {
    deck->pattern = random8(0, NUM_PATTERNS);
  }
  deck->tPatternStart = t_now;
}

void randomPalette(DeckSettings* deck, DeckSettings* otherDeck) {
  uint8_t old = deck->palette;
  while (deck->palette == old || deck->palette == otherDeck->palette) {
    deck->palette = random8(0, PALETTES_COUNT);
  }
  deck->currentPalette = palettes[deck->palette];
  deck->tPaletteStart = t_now;
}

// setup() runs once, when the device is first turned on.
void setup() {
  t_now = millis();
  t_boot = t_now;
  tLastCrossfade = t_now;

  deckSettingsA = {
    1,
    0.0,
    0,
    0,
    0,
    palettes[0],
    t_now,
    t_now,
  };
  deckSettingsB = {
    2,
    1.0,
    0,
    0,
    0,
    palettes[0],
    t_now,
    t_now,
  };
  randomPattern(&deckSettingsA, &deckSettingsB);
  randomPalette(&deckSettingsA, &deckSettingsB);
  randomPattern(&deckSettingsB, &deckSettingsA);
  randomPalette(&deckSettingsB, &deckSettingsA);

  // led controller, data pin, clock pin, RGB type (RGB is already defined in particle)
  FastLED.addLeds<LED_TYPE, LEDS_PIN>(masterOutput, NUM_LEDS);
  FastLED.setBrightness(GLOBAL_BRIGHTNESS);
  pattern_clear(masterOutput);
  pattern_clear(deckA);
  pattern_clear(deckB);
  FastLED.show();

}



void loop() {
  t_now = millis();

  // increment pattern every PATTERN_CHANGE_INTERVAL_MS, but not when a deck is active!
  if (AUTO_PATTERN_CHANGE) {
    if (t_now > deckSettingsA.tPatternStart + PATTERN_CHANGE_INTERVAL_MS && !crossfadeInProgress) {
      if (crossfadePosition == 1.0) {
        randomPattern(&deckSettingsA, &deckSettingsB);
      }
    }
    if (t_now > deckSettingsB.tPatternStart + PATTERN_CHANGE_INTERVAL_MS && !crossfadeInProgress) {
      if (crossfadePosition == 0.0) {
        randomPattern(&deckSettingsB, &deckSettingsA);
      }
    }
  }

  // increment palette every PALETTE_CHANGE_INTERVAL_MS, but not when crossfading!
  if (AUTO_CHANGE_PALETTE && !crossfadeInProgress) {
    for (int x = 0; x < VJ_NUM_DECKS ; x++) {
      int xOther = (x == 0) ? 1 : 0;
      DeckSettings* deck = deckSettingsAll[x];
      DeckSettings* otherdeck = deckSettingsAll[xOther];
      if ((deck->crossfadePositionActive != crossfadePosition) &&
          (deck->tPaletteStart + PALETTE_CHANGE_INTERVAL_MS < t_now)) {
        randomPalette(deck, otherdeck);
      }
    }
  }

  //if (t_boot + BOOTUP_ANIM_DURATION_MS > t_now) {
  //  // display a bootup pattern for a bit
  //  pattern_bootup(deckA, &deckSettingsA);
  //  for (int i = 0; i < NUM_LEDS; ++i) {
  //    deckB[i] = deckA[i];
  //  }
  //} else {
  // fill in patterns on both decks! we will crossfade master output later
  // NOTE: only render to a deck if its "visible" through the crossfader
  if ( !VJ_CROSSFADING_ENABLED || crossfadePosition < 1.0 ) {
    patternBank[deckSettingsA.pattern](deckA, &deckSettingsA);
  }
  if ( VJ_CROSSFADING_ENABLED && crossfadePosition > 0 ) {
    patternBank[deckSettingsB.pattern](deckB, &deckSettingsB);
  }
  //}

  // perform crossfading increment if we are mid pattern change
  if (VJ_CROSSFADING_ENABLED) {
    //Serial.printf("%d %d %d\n", t_now, tLastCrossfade + VJ_DECK_SWITCH_INTERVAL_MS, crossfadeInProgress);
    if (t_now > tLastCrossfade + VJ_DECK_SWITCH_INTERVAL_MS && !crossfadeInProgress) {
      // start switching between decks
      crossfadeInProgress = 1;
      tLastCrossfade = t_now;
    }
    if (crossfadeInProgress) {
      float step = (float)1.0 / (VJ_CROSSFADE_DURATION_MS / 1000.0 * UPDATES_PER_SECOND);
      // Serial.printf("fader increment %f %d\n", step, crossfadeDirection);
      crossfadePosition = crossfadePosition + crossfadeDirection * step;

      // is it time to change decks?
      // we are cut over to deck B, break this loop
      if (crossfadePosition > 1.0) {
        crossfadePosition = 1.0;
        crossfadeDirection = -1; // 1->0
        crossfadeInProgress = 0;
      }
      // we are cut over to deck B
      if (crossfadePosition < 0.0) {
        crossfadePosition = 0.0;
        crossfadeDirection = 1;  // 0->1
        crossfadeInProgress = 0;
      }
    }
  }

  // perform crossfading between deckA and deckB, by filling masterOutput
  // FIXME for now, lets just take a linear interpolation between deck a and b
  for (int i = 0; i < NUM_LEDS; ++i) {
    if (VJ_CROSSFADING_ENABLED) {
      masterOutput[i] = deckA[i].lerp8(deckB[i], fract8(255 * crossfadePosition));
    } else {
      masterOutput[i] = deckA[i];
    }
    if (t_now < + BOOTUP_ANIM_DURATION_MS) {
      // ramp intensity up slowly, so we fade in when turning on
      int8_t bri8 = (uint8_t)((t_now * 1.0) / BOOTUP_ANIM_DURATION_MS * 255.0);
      masterOutput[i] = masterOutput[i].fadeToBlackBy(255 - bri8);
    }
  }

  FastLED.setBrightness(GLOBAL_BRIGHTNESS);
  FastLED.show();
  delay(1000.0 / UPDATES_PER_SECOND);
}
