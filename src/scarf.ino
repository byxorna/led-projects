/*
* Project d18-scarf
* Description: LEDs for DI8ORIENT scarf
* Author: Gabe Conradi
* Date: idklol
*/

#include "Particle.h"
#include "FastLED.h"

FASTLED_USING_NAMESPACE;
SYSTEM_MODE(SEMI_AUTOMATIC);
// SYSTEM_THREAD(ENABLED);

struct DeckSettings {
  uint8_t label;
  float crossfadePositionActive;
  uint8_t gPattern;
  uint8_t gPalette;
  uint8_t gAnimIndex;
  CRGBPalette16 currentPalette; // current color palette
  unsigned long t_pattern_start;  // time last pattern changed
  unsigned long t_palette_start;  // time last palette changed
};

DeckSettings deckSettingsA;
DeckSettings deckSettingsB;
DeckSettings* deckSettingsAll[] = {&deckSettingsA, &deckSettingsB};

typedef void (*FP)(NSFastLED::CRGB*, DeckSettings*);

#define NUM_LEDS 102
#define LEDS_PIN D6
#define LED_TYPE NEOPIXEL
#define UPDATES_PER_SECOND 120
#define MAX_BRIGHTNESS 255
#define GLOBAL_BRIGHTNESS 80
#define MAX_SATURATION 255
#define BOOTUP_ANIM_DURATION_MS 2000
#define PATTERN_CHANGE_INTERVAL_MS 30000
#define PALETTE_CHANGE_INTERVAL_MS 30000
// switch between deck a and b with this interval
#define VJ_DECK_SWITCH_INTERVAL_MS 30000
#define AUTO_CHANGE_PALETTE 1
bool AUTO_PATTERN_CHANGE = true;
#define SETUP_BUTTON_HOLD_DURATION_MS 800
#define VJ_CROSSFADING_ENABLED 1
#define VJ_CROSSFADE_DURATION_MS 5000
#define VJ_NUM_DECKS 2

unsigned long t_now;                // time now in each loop iteration
unsigned long t_boot;               // time at bootup

/* state for controlling user-mode button for pattern changes */
uint8_t button_state = 0;
unsigned long button_timer = 0;

CFastLED* gLED; // global CFastLED object

/* custom color palettes */
// orange 255,102,0 FF6600
// pink 255,0,255 #ff00ff
// pornj 255,51,51 #ff3333
DEFINE_GRADIENT_PALETTE( Disorient_gp ) {
      0,   0,   0,   0,    // black
     75, 255,  26, 153,    // pink
    147, 255,  51,  51,    // pornj
    208, 255, 111,  15,    // orange
    255, 255, 255, 255, }; // white

DEFINE_GRADIENT_PALETTE( Disorient_Dark_gp ) {
     0,  255,  26, 153,    // pink
     50,   0,   0,   0,    // black
    100, 255,  255,  255,  // white
    150,   0,   0,   0,    // black
    200, 255, 111,  15,    // orange
    255,   0,   0,   0, }; // black


// for effects that are palette based
CRGBPalette16 palettes[6] = {
  Disorient_gp,
  RainbowColors_p,
  CloudColors_p,
  ForestColors_p,
  OceanColors_p,
  LavaColors_p,
};


TBlendType currentBlending = LINEARBLEND;
CRGB masterOutput[NUM_LEDS];
CRGB deckA[NUM_LEDS];
CRGB deckB[NUM_LEDS];
float crossfadePosition = 1.0;  // 0.0 is deckA, 1.0 is deckB
int crossfadeDirection = (crossfadePosition == 1.0) ? -1 : 1; // start going B -> A
uint8_t crossfadeInProgress = 0;
unsigned long tLastCrossfade = 0;

void pattern_slow_pulse_with_sparkles(NSFastLED::CRGB* leds, DeckSettings* s) {
  // pick a color, and pulse it 
  uint8_t cBrightness = beatsin8(20, 120, 255);
  uint8_t cHue = beatsin8(4, 0, 255);
  CHSV hsv_led = CHSV(cHue, 255, cBrightness);
  CRGB rgb_led;
  hsv2rgb_rainbow(hsv_led, rgb_led);
  for( int i = 0; i < NUM_LEDS; i++) {
    if (random(NUM_LEDS*3) == 0) {
      leds[i] = CRGB::White;
    } else {
      leds[i] = rgb_led;
    }
  }
}

void pattern_cylon_eye(NSFastLED::CRGB* leds, DeckSettings* s) {
  // cylon eye is 4 pixels wide, +/++ base index
  // we map a 60bpm(1s) cycle into 0..num leds-1
  uint8_t h = beatsin8(8, 0, 255);
  CHSV hsv_led = CHSV(h, 255, 255);
  CRGB rgb_led;
  hsv2rgb_rainbow(hsv_led, rgb_led);
  uint8_t mappedIndex = beatsin8(60, 0, NUM_LEDS-1);
  for(int i = 0; i < NUM_LEDS; ++i) {
    if (mappedIndex == i) {
      leds[i] = rgb_led;
    } else if (addmod8(mappedIndex, 1, 255) == i) {
      leds[i] = rgb_led;
    } else if (addmod8(mappedIndex, 2, 255) == i) {
      leds[i] = rgb_led;
    } else if (addmod8(mappedIndex, 3, 255) == i) {
      leds[i] = rgb_led;
    } else {
      leds[i] = CRGB::Black;
    }
  }
}

void pattern_bootup_with_sparkles(NSFastLED::CRGB* leds, DeckSettings* s) {
  uint8_t baseHue = beatsin8(15, 0, 255);
  uint8_t iHue = 0;
  for(int i = 0; i < NUM_LEDS; ++i) {
    if (random(NUM_LEDS) == 0) {
      leds[i] = CRGB::White;
    } else {
      iHue = addmod8(baseHue, 1, 255);
      CHSV hsv_led = CHSV(iHue, 255, 255);
      CRGB rgb_led;
      hsv2rgb_rainbow(hsv_led, rgb_led);
      leds[i] = rgb_led;
    }
  }
}

// cycle a rainbow, varying how quickly it rolls around the board
void pattern_rainbow_waves_with_sparkles(NSFastLED::CRGB* leds, DeckSettings* s) {
  for(int i = 0; i < NUM_LEDS; ++i) {
    if (random(NUM_LEDS*3) == 0) {
      leds[i] = CRGB::White;
    } else {
      uint8_t h = (t_now/12+i)%256;
      CHSV hsv_led = CHSV(h, 255, 255);
      CRGB rgb_led;
      hsv2rgb_rainbow(hsv_led, rgb_led);
      leds[i] = rgb_led;
    }
  }
}

void pattern_clear(NSFastLED::CRGB* leds) {
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
}

void pattern_disorient_palette_sparkles(NSFastLED::CRGB* leds, DeckSettings* s) {
  uint8_t b = beatsin8(4, 0, 255);
  for( int i = 0; i < NUM_LEDS; i++) {
    if (random(NUM_LEDS*4) == 0) {
      leds[i] = CRGB::White;
    } else {
      leds[i] = ColorFromPalette((CRGBPalette16)Disorient_gp, s->gAnimIndex + i + b, MAX_BRIGHTNESS, currentBlending);
    }
  }
  // slow down progression by 1/3
  if (t_now%3 == 0) {
    s->gAnimIndex = addmod8(s->gAnimIndex, 1, 255);
  }
}

// undulates a color wave, an offset into that wave, and intensity of the led
/* im bad at programming and cant figure out the noise functions
void pattern_time_stretch_waves_rainbow(NSFastLED::CRGB* leds, DeckSettings* s){
  float speedScale = .1;
  float speedColor = 0.5;
  for( int i = 0; i < NUM_LEDS; i++) {
    uint8_t stretchOffset = map8( inoise8((t_now + i)), 0, 16);
    //uint8_t hue = inoise8(t_now, random16(), i);
    float hraw = t_now*speedColor/1000.0 + 1.0*i/10.0;
    uint8_t hue = cos8(hraw);
    uint8_t intensity = cos8(t_now*speedScale + 1.0);
    //Serial.printlnf("%3d %d %d",i, hue, intensity);
    Serial.printlnf("%3d %3d %3d",i, hue, hraw);

    CRGB rgb_led;
    CHSV hsv_led = CHSV(hue, 255, intensity);
    hsv2rgb_rainbow(hsv_led, rgb_led);
    leds[i] = rgb_led;
  }
}
*/

void pattern_from_palette(NSFastLED::CRGB* leds, DeckSettings* s) {
  uint8_t b = beatsin8(4, 0, 255);
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(s->currentPalette, s->gAnimIndex + i + b, MAX_BRIGHTNESS, currentBlending);
  }
  // slow down progression by 1/3
  if (t_now%3 == 0) {
    s->gAnimIndex = addmod8(s->gAnimIndex, 1, 255);
  }
}

void pattern_brake_light(NSFastLED::CRGB* leds, DeckSettings* s) {
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CRGB::Red;
  }
}

// NOTE: lifted and tweaked from https://learn.adafruit.com/rainbow-chakra-led-hoodie/the-code
// This function draws color waves with an ever-changing,
// widely-varying set of parameters, using a color palette.
void pattern_palette_waves(NSFastLED::CRGB* leds, DeckSettings* s) {
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

    nblend(leds[pixelnumber], newcolor, 128);
  }
}

// setup() runs once, when the device is first turned on.
void setup() {
  t_now = millis();
  t_boot = t_now;
  tLastCrossfade = t_now;
  Serial.begin(9600);
  Serial.println("resetting");

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



  // led controller, data pin, clock pin, RGB type (RGB is already defined in particle)
  gLED = new CFastLED();
  gLED->addLeds<LED_TYPE, LEDS_PIN>(masterOutput, NUM_LEDS);
  gLED->setBrightness(GLOBAL_BRIGHTNESS);
  pattern_clear(masterOutput);
  // NOTE: deckA and deckB are only used to perform crossfaded transitions
  pattern_clear(deckA);
  pattern_clear(deckB);
  gLED->show();

  Serial.println("booted up");
}

/** update this with patterns you want to be cycled through **/
#define NUM_PATTERNS sizeof(patternBank) / sizeof(FP)
const FP patternBank[] = {
  //&pattern_time_stretch_waves_rainbow,
  &pattern_from_palette,
  &pattern_disorient_palette_sparkles,
  &pattern_slow_pulse_with_sparkles,
  &pattern_palette_waves,
  &pattern_rainbow_waves_with_sparkles,
};

void loop() {
  t_now = millis();

  // handle user interaction with reset button
  if (HAL_Core_Mode_Button_Pressed(SETUP_BUTTON_HOLD_DURATION_MS)) {
    switch (button_state) {
    case 0:
      // we werent pressed before, so start timer!
      button_state = 1;
      button_timer = t_now;
      //RGB.color(255, 0, 0);
      break;
    case 1:
      //RGB.color(0, 255, 0); // green when waiting
      if (t_now - button_timer > SETUP_BUTTON_HOLD_DURATION_MS) {
        // we have been held longer than
        button_state = 2;
      }
      break;
    // we are waiting to take action now
    case 2: break;
    // action already taken, do nothing until release!
    case 3: break;
    default:
        button_state = 0;
      break;
    }
  } else {
    button_state = 0;
  }
  if (button_state == 2) {
    // disable auto pattern changing now!
    deckSettingsA.gPattern++;
    deckSettingsB.gPattern++;
    button_state = 3;
    if (AUTO_PATTERN_CHANGE) {
      AUTO_PATTERN_CHANGE = false;
      Serial.printlnf("moving to fixed pattern mode (use button to change)");
    }
    Serial.printlnf("deckA.pattern=%d", deckSettingsA.gPattern);
    Serial.printlnf("deckB.pattern=%d", deckSettingsB.gPattern);
  }

  // increment pattern every PATTERN_CHANGE_INTERVAL_MS, but not when a deck is active!
  if (AUTO_PATTERN_CHANGE && !crossfadeInProgress) {
    if (t_now > deckSettingsA.t_pattern_start+PATTERN_CHANGE_INTERVAL_MS) {
      if (crossfadePosition == 1.0) {
        deckSettingsA.gPattern++;
        deckSettingsA.t_pattern_start = t_now;
        if (deckSettingsA.gPattern >= NUM_PATTERNS) {
          deckSettingsA.gPattern = 0;
        }
        Serial.printlnf("deckA.pattern=%d", deckSettingsA.gPattern);
      }
    }
    if (t_now > deckSettingsB.t_pattern_start+PATTERN_CHANGE_INTERVAL_MS) {
      if (crossfadePosition == 0.0) {
        deckSettingsB.gPattern++;
        deckSettingsB.t_pattern_start = t_now;
        if (deckSettingsB.gPattern >= NUM_PATTERNS) {
          deckSettingsB.gPattern = 0;
        }
        Serial.printlnf("deckB.pattern=%d", deckSettingsB.gPattern);
      }
    }
  }

  // increment palette every PALETTE_CHANGE_INTERVAL_MS, but not when crossfading!
  if (AUTO_CHANGE_PALETTE && !crossfadeInProgress) {
    for (int x = 0; x < VJ_NUM_DECKS ; x++){
      DeckSettings* deck = deckSettingsAll[x];
      if ((deck->crossfadePositionActive != crossfadePosition) && 
        (deck->t_palette_start + PALETTE_CHANGE_INTERVAL_MS < t_now)) {
        deck->gPalette++;
        if (deck->gPalette >= (sizeof(palettes)/sizeof(*palettes))) {
          deck->gPalette = 0;
        }
        deck->currentPalette = palettes[deck->gPalette];
        deck->t_palette_start = t_now;
        Serial.printlnf("deck%d.palette=%d", deck->label, deck->gPalette);
      }
    }
  }

  if (t_boot + BOOTUP_ANIM_DURATION_MS > t_now) {
    // display a bootup pattern for a bit
    pattern_bootup_with_sparkles(deckA, &deckSettingsA);
    for (int i = 0; i < NUM_LEDS; ++i) {
      deckB[i] = deckA[i];
    }
  } else {
    // fill in patterns on both decks! we will crossfade master output later
    // NOTE: only render to a deck if its "visible" through the crossfader
    if ( !VJ_CROSSFADING_ENABLED || crossfadePosition < 1.0 ) {
      patternBank[deckSettingsA.gPattern](deckA, &deckSettingsA);
    }
    if ( VJ_CROSSFADING_ENABLED && crossfadePosition > 0 ) {
      patternBank[deckSettingsB.gPattern](deckB, &deckSettingsB);
    }
  }

  // perform crossfading increment if we are mid pattern change
  if (VJ_CROSSFADING_ENABLED) {
    //Serial.printf("%d %d %d\n", t_now, tLastCrossfade + VJ_DECK_SWITCH_INTERVAL_MS, crossfadeInProgress);
    if (t_now > tLastCrossfade + VJ_DECK_SWITCH_INTERVAL_MS && !crossfadeInProgress) {
      // start switching between decks
      Serial.printf("starting fading to %c\n", (crossfadePosition == 1.0) ? 'A' : 'B');
      crossfadeInProgress = 1;
      tLastCrossfade = t_now;
    }
    if (crossfadeInProgress) {
      float step = (float)1.0/(VJ_CROSSFADE_DURATION_MS/1000*UPDATES_PER_SECOND);
      // Serial.printf("fader increment %f %d\n", step, crossfadeDirection);
      crossfadePosition = crossfadePosition + crossfadeDirection * step;

      // is it time to change decks?
      // we are cut over to deck B, break this loop
      if (crossfadePosition > 1.0) {
        crossfadePosition = 1.0;
        crossfadeDirection = -1; // 1->0
        crossfadeInProgress = 0;
        Serial.printf("finished fading to B\n");
      }
      // we are cut over to deck B
      if (crossfadePosition < 0.0) {
        crossfadePosition = 0.0;
        crossfadeDirection = 1;  // 0->1
        crossfadeInProgress = 0;
        Serial.printf("finished fading to A\n");
      }
    }
  }
  /*
  if (crossfadeInProgress) {
    Serial.printf("fader %0.2f %d\n", crossfadePosition, crossfadeInProgress);
  }
  */

  // perform crossfading between deckA and deckB, by filling masterOutput
  // FIXME for now, lets just take a linear interpolation between deck a and b
  for (int i = 0; i < NUM_LEDS; ++i) {
    if (VJ_CROSSFADING_ENABLED) {
      masterOutput[i] = deckA[i].lerp8(deckB[i], fract8(255*crossfadePosition));
      //masterOutput[i] = deckA[i];
    } else {
      masterOutput[i] = deckA[i];
    }
  }

  gLED->show();
  delay(1000.0 / UPDATES_PER_SECOND);
}
