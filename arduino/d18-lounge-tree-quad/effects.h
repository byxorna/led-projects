/*vars for pattern_phase_shift_palette*/
int wave1 = 0;
void pattern_phase_shift_palette(CRGB* leds, DeckSettings* s) {
  // phase shift
  wave1 += 8;
  int phase2 = beatsin8(7, -64, 64);

  for (int k = 0; k < NUM_LEDS; k++) {
    int phase1 = sin8(3 * k + wave1 / 128);
    int colorIndex = cubicwave8((k) + phase1) / 2 + cos8((k * 3) + phase2) / 2;

    //int bri8 = cubicwave8(t_now/10.0 + k*10.0); // nice pulsy one direction intensity modulator
    // generate undulating intensity phases
    int bri8 = cubicwave8(t_now / 10.0 + cubicwave8(k * 10.0));

    leds[k] = ColorFromPalette(s->currentPalette, colorIndex, bri8, currentBlending);
  }
}

void  pattern_plasma(CRGB* leds, DeckSettings* s) {

  int thisPhase = beatsin8(6, -64, 64);
  int thatPhase = beatsin8(7, -64, 64);

  for (int k = 0; k < NUM_LEDS; k++) {

    int colorIndex = cubicwave8((k * 23) + thisPhase) / 2 + cos8((k * 15) + thatPhase) / 2;
    int thisBright = cubicwave8(t_now / 10.0 + k * 10.0); // nice pulsy one direction intensity modulator
    //int thisBright = qsuba(colorIndex, beatsin8(7,0,96));

    leds[k] = ColorFromPalette(s->currentPalette, colorIndex, thisBright, currentBlending);
  }
}

void pattern_slow_pulse(CRGB* leds, DeckSettings* s) {
  // pick a color, and pulse it
  //uint8_t cBrightness = beatsin8(20, 120, 255);
  uint8_t cHue = beatsin8(4, 0, 255);
  for ( int i = 0; i < NUM_LEDS; i++) {
    int thisBright = cubicwave8(t_now / 10.0 + i * 10.0); // nice pulsy one direction intensity modulator
    CHSV hsv_led = CHSV(cHue, 255, thisBright);
    CRGB rgb_led;
    hsv2rgb_rainbow(hsv_led, rgb_led);
    leds[i] = rgb_led;
  }
}

void pattern_cylon_eye(CRGB* leds, DeckSettings* s) {
  // cylon eye is 4 pixels wide, +/++ base index
  // we map a 60bpm(1s) cycle into 0..num leds-1
  uint8_t h = beatsin8(8, 0, 255);
  CHSV hsv_led = CHSV(h, 255, 255);
  CRGB rgb_led;
  hsv2rgb_rainbow(hsv_led, rgb_led);
  uint8_t mappedIndex = beatsin8(60, 0, NUM_LEDS - 1);
  for (int i = 0; i < NUM_LEDS; ++i) {
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

void pattern_bootup(CRGB* leds, DeckSettings* s) {
  // ramp intensity up as we boot up. act like we are warming up
  float intensity = (t_now / BOOTUP_ANIM_DURATION_MS) * 255.0;
  uint8_t v = 255;
  if (intensity >= 255.0) {
    v = 255;
  }
  uint8_t baseHue = beatsin8(3, 0, 255);
  uint8_t iHue = 0;
  for (int i = 0; i < NUM_LEDS; ++i) {
    iHue = addmod8(baseHue, 1, 255);
    CHSV hsv_led = CHSV(iHue, 255, v);
    CRGB rgb_led;
    hsv2rgb_rainbow(hsv_led, rgb_led);
    leds[i] = rgb_led;
  }
}

// cycle a rainbow, varying how quickly it rolls around the board
void pattern_rainbow_waves(CRGB* leds, DeckSettings* s) {
  for (int i = 0; i < NUM_LEDS; ++i) {
    uint8_t h = (t_now / 12 + i) % 256;
    CHSV hsv_led = CHSV(h, 255, 255);
    CRGB rgb_led;
    hsv2rgb_rainbow(hsv_led, rgb_led);
    leds[i] = rgb_led;
  }
}

void pattern_clear(CRGB* leds) {
  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
}
void pattern_from_palette(CRGB* leds, DeckSettings* s) {
  uint8_t b = beatsin8(4, 0, 255);
  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(s->currentPalette, s->animationIndex + i + b, 255, currentBlending);
  }
  // slow down progression by 1/3
  if (t_now % 3 == 0) {
    s->animationIndex = addmod8(s->animationIndex, 1, 255);
  }
}

void pattern_brake_light(CRGB* leds, DeckSettings* s) {
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CRGB::Red;
  }
}

// NOTE: lifted and tweaked from https://learn.adafruit.com/rainbow-chakra-led-hoodie/the-code
// This function draws color waves with an ever-changing,
// widely-varying set of parameters, using a color palette.
void pattern_palette_waves(CRGB* leds, DeckSettings* s) {
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
  sHue16 += deltams * beatsin88( 400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for ( uint16_t i = 0 ; i < numleds; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;
    uint16_t h16_128 = hue16 >> 7;
    if ( h16_128 & 0x100) {
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
    pixelnumber = (numleds - 1) - pixelnumber;

    nblend(leds[pixelnumber], newcolor, 128);
  }
}

/** update this with patterns you want to be cycled through **/
#define NUM_PATTERNS sizeof(patternBank) / sizeof(FP)
const FP patternBank[] = {
  &pattern_plasma,
  //&pattern_from_palette,
  &pattern_phase_shift_palette,
  //&pattern_slow_pulse,
  &pattern_palette_waves,
  //&pattern_rainbow_waves,
 
};
