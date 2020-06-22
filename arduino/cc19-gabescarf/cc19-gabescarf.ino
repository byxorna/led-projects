/*
* Project cc19-scarf
* Description: LEDs for DI8ORIENT scarf - Country Club 2019
* Author: Gabe Conradi
* Date: idklol
*/

// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b)  ((x>b)?b:0)
#define qsuba(x, b)  ((x>b)?x-b:0)
#define NUM_LEDS 128
#define LEDS_PIN 3
#define LED_TYPE NEOPIXEL
#define UPDATES_PER_SECOND 40
#define MAX_BRIGHTNESS 255
#define MAX_SATURATION 255
#define BOOTUP_ANIM_DURATION_MS 4000
#define PATTERN_CHANGE_INTERVAL_MS 30000
#define PALETTE_CHANGE_INTERVAL_MS 30000
#define EFFECT_CHANGE_INTERVAL_MS 10003
#define VJ_CROSSFADING_ENABLED 1
#define VJ_CROSSFADE_DURATION_MS 4000
#define VJ_NUM_DECKS 2
// switch between deck a and b with this interval
#define VJ_DECK_SWITCH_INTERVAL_MS 15000
#define SETUP_BUTTON_HOLD_DURATION_MS 800
bool AUTO_CHANGE_PALETTE = true;

#ifndef _FASTLED_H_
#include "FastLED.h"
#endif
#ifndef _STRUCTS_H_
#include "structs.h"
#endif
#ifndef _PALETTES_H_
#include "palettes.h"
#endif
#ifndef _EFFECTS_H_
#include "effects.h"
#endif

Deck MasterOutput;
Deck DeckA;
Deck DeckB;
Deck* DeckAll[] = {&DeckA, &DeckB};
Mixer mainMixer;

typedef void (*DrawFunction)(Deck*);
// how 2 decks mix together into an output
typedef void (*MixerFunction)(Deck*, Deck*, Deck*);

const uint8_t BRIGHTNESS_VALUES[] = {40, 120, 255};
#define BRIGHTNESS_COUNT sizeof(BRIGHTNESS_VALUES)/sizeof(uint8_t)
#define GLOBAL_BRIGHTNESS BRIGHTNESS_VALUES[BRIGHTNESS_INDEX]
const uint8_t BRIGHTNESS_INDEX = 1;
const bool AUTO_PATTERN_CHANGE = true;

unsigned long t_now;                // time now in each loop iteration
unsigned long t_boot;               // time at bootup

/* state for controlling user-mode button for pattern changes */
uint8_t button_state = 0;
unsigned long button_timer = 0;

TBlendType currentBlending = LINEARBLEND;
CRGB leds[NUM_LEDS];
CRGB ledsA[NUM_LEDS];
CRGB ledsB[NUM_LEDS];

struct Deck;
struct Mixer;

void pattern_slow_pulse(Deck* s) {
  // pick a color, and pulse it 
  uint8_t cBrightness = beatsin8(10, 140, 255);
  uint8_t cHue = beatsin8(4, 0, 255);
  CHSV hsv_led = CHSV(cHue, 255, cBrightness);
  CRGB rgb_led;
  hsv2rgb_rainbow(hsv_led, rgb_led);
  for( int i = 0; i < NUM_LEDS; i++) {
    s->leds[i] = rgb_led;
  }
}

void  pattern_plasma(Deck* s) {

  int thisPhase = beatsin88(256*2,-64,64);
  int thatPhase = beatsin88(237,-64,64);

  for (int k=0; k<NUM_LEDS; k++) {

    int colorIndex = cubicwave8((k*17)+thisPhase)/2 + cos8((k*15)+thatPhase)/2;
    int thisBright = cubicwave8(t_now/10.0 + k*10.0); // nice pulsy one direction intensity modulator
    //int thisBright = qsuba(colorIndex, beatsin8(7,0,96));

    s->leds[k] = ColorFromPalette(s->currentPalette, colorIndex, thisBright, currentBlending);
  }
}

// cycle a rainbow, varying how quickly it rolls around the board
void pattern_rainbow_waves(Deck* s) {
  for(int i = 0; i < NUM_LEDS; ++i) {
    uint8_t h = (t_now/12+i)%256;
    CHSV hsv_led = CHSV(h, 255, 255);
    CRGB rgb_led;
    hsv2rgb_rainbow(hsv_led, rgb_led);
    s->leds[i] = rgb_led;
  }
}

void pattern_clear(Deck* s) {
  for( int i = 0; i < NUM_LEDS; i++) {
    s->leds[i] = CRGB::Black;
  }
}

// NOTE: lifted and tweaked from https://learn.adafruit.com/rainbow-chakra-led-hoodie/the-code
// This function draws color waves with an ever-changing,
// widely-varying set of parameters, using a color palette.
void pattern_palette_waves(Deck* s) {
  uint8_t numleds = NUM_LEDS;
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  //uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 300, 1500);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;

  for( uint16_t i = 0 ; i < numleds; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;
    uint16_t h16_128 = hue16 >> 7;
    if( h16_128 & 0x100) {
      hue8 = 255 - (h16_128 >> 1);
    } else {
      hue8 = h16_128 >> 1;
    }

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    uint8_t index = hue8;
    index = scale8( index, 240);

    CRGB newcolor = ColorFromPalette(s->currentPalette, index, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (numleds-1) - pixelnumber;

    nblend(s->leds[pixelnumber], newcolor, 128);
  }
}

/** update this with patterns you want to be cycled through **/
#define NUM_PATTERNS sizeof(patternBank) / sizeof(DrawFunction)
const DrawFunction patternBank[] = {
  &pattern_palette_waves,
  &pattern_palette_waves,
  &pattern_rainbow_waves,
};

#define NUM_EFFECTS sizeof(effectBank) / sizeof(EffectFunction)
const EffectFunction effectBank[] = {
  NULL,
  &effect_reverse,
  &effect_mirror,
  &effect_reverse_mirror,
};

// change dw/p1/p2 on some period
/*
void stepFxParams(Mixer* m) {
  m->fxDryWet = beatsin8(12, 0, 255);
  m->fxParam1 = beatsin8(12, 0, 255, 0, m->fxDryWet );
  m->fxParam2 = beatsin8(19, 0, 255, 0, 0);
}
*/

void randomEffect(Deck* d) {
  uint8_t old = d->fxEffectIndex;
  while (d->fxEffectIndex == old) {
    // pick a new effect that isnt the old effect index
    d->fxEffectIndex = random8(0, NUM_EFFECTS);
  }
  d->tFxEffectStart = t_now;
}

/*
void randomEffect(Mixer* m) {
  uint8_t old = m->fxEffectIndex;
  while (m->fxEffectIndex == old) {
    // pick a new effect that isnt the old effect index
    m->fxEffectIndex = random8(0, NUM_EFFECTS);
  }
  m->tFxEffectStart = t_now;
}
*/

void randomPattern(Deck* deck, Deck* otherDeck) {
  uint8_t old = deck->pattern;
  while (deck->pattern == old || deck->pattern == otherDeck->pattern) {
    deck->pattern = random8(0, NUM_PATTERNS);
  }
  deck->tPatternStart = t_now;
}

void usePattern(Deck* deck, uint8_t patternIndex){
  if (patternIndex >= NUM_PATTERNS) {
    patternIndex = 0;
  }
  deck->pattern = patternIndex;
  deck->tPatternStart = t_now;
}

void randomPalette(Deck* deck, Deck* otherDeck) {
  uint8_t old = deck->palette;
  while (deck->palette == old || deck->palette == otherDeck->palette) {
    deck->palette = random8(0, PALETTES_COUNT);
  }
  deck->currentPalette = palettes[deck->palette];
  deck->tPaletteStart = t_now;
}

void usePalette(Deck* deck, uint8_t paletteIndex){
  if (paletteIndex >= PALETTES_COUNT) {
    paletteIndex = 0;
  }
  deck->palette = paletteIndex;
  deck->currentPalette = palettes[deck->palette];
  deck->tPaletteStart = t_now;
}

void mixer_crossfade_blend(Mixer* mixer, Deck* a, Deck* b, Deck* out) {
  for (int i = 0; i < NUM_LEDS; ++i) {
    out->leds[i] = a->leds[i].lerp8(b->leds[i], fract8(255*mixer->crossfadePosition));
    if (t_now < + BOOTUP_ANIM_DURATION_MS) {
      // ramp intensity up slowly, so we fade in when turning on
      int8_t bri8 = (uint8_t)((t_now*1.0)/BOOTUP_ANIM_DURATION_MS*255.0);
      out->leds[i] = out->leds[i].fadeToBlackBy(255-bri8);
    }
  }
}


// setup() runs once, when the device is first turned on.
void setup() {
  t_now = millis();
  t_boot = t_now;
  Serial.begin(9600);
  Serial.println("resetting");

  MasterOutput = {
    1,
    0.0,
    0,
    0,
    0,
    palettes[0],
    t_now,
    t_now,
    leds,
    0,  // fx effect index in effectBank
    t_now, // time fx was changed
  };

  DeckA = {
    1,
    0.0,
    0,
    0,
    0,
    palettes[0],
    t_now,
    t_now,
    ledsA,
    0,  // fx effect index in effectBank
    t_now, // time fx was changed
  };

  DeckB = {
    2,
    1.0,
    0,
    0,
    0,
    palettes[0],
    t_now,
    t_now,
    ledsB,
    0,  // fx effect index in effectBank
    t_now, // time fx was changed
  };

  mainMixer = {
    1.0,  // position; 0.0 is deckA, 1.0 is deckB
    -1, // start fading direction going B -> A
    false, // crossfade in progress
    0, // last crossfade
    0,  // fx effect index in effectBank
    0,  // fx d/w
    0,  // fx p1
    0,  // fx p2
    0,  // time fx effect started
    &DeckA,
    &DeckB,
    &MasterOutput,
  };

  randomPattern(&DeckA, &DeckB);
  randomPalette(&DeckA, &DeckB);
  randomPattern(&DeckB, &DeckA);
  randomPalette(&DeckB, &DeckA);
  randomEffect(&DeckA);
  randomEffect(&DeckB);

  // led controller, data pin, clock pin, RGB type (RGB is already defined in particle)
  /*
  gLED = new CFastLED();
  gLED->addLeds<LED_TYPE, LEDS_PIN>(MasterOutput.leds, NUM_LEDS);
  gLED->setBrightness(GLOBAL_BRIGHTNESS);
  pattern_clear(&DeckA);
  pattern_clear(&DeckB);
  gLED->show();
  */

  FastLED.addLeds<LED_TYPE, LEDS_PIN>(MasterOutput.leds, NUM_LEDS);
  FastLED.setBrightness(GLOBAL_BRIGHTNESS);
  pattern_clear(&MasterOutput);
  pattern_clear(&DeckA);
  pattern_clear(&DeckB);
  FastLED.show();

  Serial.println("booted up");
}

void loop() {
  t_now = millis();


  // increment pattern every PATTERN_CHANGE_INTERVAL_MS, but not when a deck is active!
  if (AUTO_PATTERN_CHANGE) {
    if (t_now > DeckA.tPatternStart+PATTERN_CHANGE_INTERVAL_MS && !mainMixer.crossfadeInProgress) {
      if (mainMixer.crossfadePosition == 1.0) {
        randomPattern(&DeckA, &DeckB);
        //Serial.printlnf("deckA.pattern=%d", DeckA.pattern);
      }
    }
    if (t_now > DeckB.tPatternStart+PATTERN_CHANGE_INTERVAL_MS && !mainMixer.crossfadeInProgress) {
      if (mainMixer.crossfadePosition == 0.0) {
        randomPattern(&DeckB, &DeckA);
        //Serial.printlnf("deckB.pattern=%d", DeckB.pattern);
      }
    }
  }

  // increment palette every PALETTE_CHANGE_INTERVAL_MS, but not when crossfading!
  if (AUTO_CHANGE_PALETTE && !mainMixer.crossfadeInProgress) {
    for (int x = 0; x < VJ_NUM_DECKS ; x++){
      int xOther = (x == 0) ? 1 : 0;
      Deck* deck = DeckAll[x];
      Deck* otherdeck = DeckAll[xOther];
      if ((deck->crossfadePositionActive != mainMixer.crossfadePosition) &&
          (deck->tPaletteStart + PALETTE_CHANGE_INTERVAL_MS < t_now)) {
        randomPalette(deck, otherdeck);
        //Serial.printlnf("deck%d.palette=%d", deck->label, deck->palette);
      }
    }
  }



  // fill in patterns on both decks! we will crossfade master output later
  // NOTE: only render to a deck if its "visible" through the crossfader
  //if ( !VJ_CROSSFADING_ENABLED || mainMixer.crossfadePosition < 1.0 ) {
    patternBank[DeckA.pattern](&DeckA);
  //}
  //if ( VJ_CROSSFADING_ENABLED && mainMixer.crossfadePosition > 0 ) {
    patternBank[DeckB.pattern](&DeckB);
  //}

  // perform crossfading increment if we are mid pattern change
  if (VJ_CROSSFADING_ENABLED) {
    if (t_now > mainMixer.tLastCrossfade + VJ_DECK_SWITCH_INTERVAL_MS && !mainMixer.crossfadeInProgress) {
      // start switching between decks
      //Serial.printf("starting fading to %c\n", (mainMixer.crossfadePosition == 1.0) ? 'A' : 'B');
      mainMixer.crossfadeInProgress = true;
      mainMixer.tLastCrossfade = t_now;
    }
    if (mainMixer.crossfadeInProgress) {
      float step = (float)1.0/(VJ_CROSSFADE_DURATION_MS/1000*UPDATES_PER_SECOND);
      mainMixer.crossfadePosition = mainMixer.crossfadePosition + mainMixer.crossfadeDirection * step;

      // is it time to change decks?
      // we are cut over to deck B, break this loop
      if (mainMixer.crossfadePosition >= 1.0) {
        mainMixer.crossfadePosition = 1.0;
        mainMixer.crossfadeDirection = -1; // 1->0
        mainMixer.crossfadeInProgress = false;
        //Serial.printf("finished fading to B\n");
      }
      // we are cut over to deck B
      if (mainMixer.crossfadePosition <= 0.0) {
        mainMixer.crossfadePosition = 0.0;
        mainMixer.crossfadeDirection = 1;  // 0->1
        mainMixer.crossfadeInProgress = false;
        //Serial.printf("finished fading to A\n");
      }
    }
  }

  // perform prefader effects
  if (effectBank[DeckA.fxEffectIndex] != NULL) {
    effectBank[DeckA.fxEffectIndex](&DeckA, mainMixer.fxDryWet, mainMixer.fxParam1, mainMixer.fxParam2);
  }
  if (effectBank[DeckB.fxEffectIndex] != NULL) {
    effectBank[DeckB.fxEffectIndex](&DeckB, mainMixer.fxDryWet, mainMixer.fxParam1, mainMixer.fxParam2);
  }

  // perform crossfading
  mixer_crossfade_blend(&mainMixer, &DeckA, &DeckB, &MasterOutput);

  if (effectBank[mainMixer.fxEffectIndex] != NULL) {
    effectBank[mainMixer.fxEffectIndex](&MasterOutput, mainMixer.fxDryWet, mainMixer.fxParam1, mainMixer.fxParam2);
  }

  // pick new effects if we should
  if (!mainMixer.crossfadeInProgress) {
    if (DeckA.tFxEffectStart + EFFECT_CHANGE_INTERVAL_MS < t_now) {
      if (mainMixer.crossfadePosition == 1.0) {
        randomEffect(&DeckA);
        //Serial.printlnf("deckA.effect=%d (%p)", DeckA.fxEffectIndex, effectBank[DeckA.fxEffectIndex]);
      }
    }
    if (DeckB.tFxEffectStart + EFFECT_CHANGE_INTERVAL_MS < t_now) {
      if (mainMixer.crossfadePosition == 0.0) {
        randomEffect(&DeckB);
        //Serial.printlnf("deckB.effect=%d (%p)", DeckB.fxEffectIndex, effectBank[DeckB.fxEffectIndex]);
      }
    }
  }
  //stepFxParams(&mainMixer);


  FastLED.setBrightness(GLOBAL_BRIGHTNESS);
  FastLED.show();
  delay(1000.0 / UPDATES_PER_SECOND);
}
