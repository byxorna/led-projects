#ifndef _PALETTES_H_
#define _PALETTES_H_

#ifndef _PARTICLE_H_
#include "Particle.h"
#endif
#ifndef _FASTLED_H_
#include "FastLED.h"
#endif

/* custom color palettes */
// orange 255,102,0 FF6600
// pink 255,0,255 #ff00ff
// pornj 255,51,51 #ff3333
extern const NSFastLED::TProgmemRGBGradientPalette_byte Disorient_gp[] = {
      0,   0,   0,   0,    // black
     75, 255,  26, 153,    // pink
    147, 255,  51,  51,    // pornj
    208, 255, 111,  15,    // orange
    255, 255, 255, 255, }; // white

// Gradient palette "bhw1_01_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_01.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 12 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte  bhw1_01_gp [] {
    0, 227,101,  3,
  117, 194, 18, 19,
  255,  92,  8,192};

// Gradient palette "bhw1_04_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_04.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte  bhw1_04_gp [] {
    0, 229,227,  1,
   15, 227,101,  3,
  142,  40,  1, 80,
  198,  17,  1, 79,
  255,   0,  0, 45};

// Gradient palette "bhw3_50_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw3/tn/bhw3_50.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte  bhw3_50_gp [] {
    0, 171,  1, 34,
   25, 224, 29, 84,
   58, 222, 70,228,
   89, 227, 90, 59,
  127, 255,136,125,
  165, 227, 90, 59,
  188, 222, 70,228,
  216, 224, 29, 84,
  255, 171,  1, 34};

// Gradient palette "bhw3_52_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw3/tn/bhw3_52.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte bhw3_52_gp [] {
    0,  31,  1, 27,
   45,  34,  1, 16,
   99, 137,  5,  9,
  132, 213,128, 10,
  175, 199, 22,  1,
  201, 199,  9,  6,
  255,   1,  0,  1};

// Gradient palette "bhw3_02_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw3/tn/bhw3_02.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte bhw3_02_gp [] {
    0, 121,  1,  1,
   63, 255, 57,  1,
  112, 255, 79, 25,
  145, 255, 79, 25,
  188, 244,146,  3,
  255, 115, 14,  1};


// https://gist.github.com/kriegsman/8281905786e8b2632aeb

// Gradient palette "es_pinksplash_08_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/pink_splash/tn/es_pinksplash_08.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.
extern const NSFastLED::TProgmemRGBGradientPalette_byte es_pinksplash_08_gp[] = {
    0, 126, 11,255,
  127, 197,  1, 22,
  175, 210,157,172,
  221, 157,  3,112,
  255, 157,  3,112};

// Gradient palette "es_pinksplash_07_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/pink_splash/tn/es_pinksplash_07.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte es_pinksplash_07_gp[] = {
    0, 229,  1,  1,
   61, 242,  4, 63,
  101, 255, 12,255,
  127, 249, 81,252,
  153, 255, 11,235,
  193, 244,  5, 68,
  255, 232,  1,  5};

// Gradient palette "Sunset_Real_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Sunset_Real.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte Sunset_Real_gp[] {
    0, 120,  0,  0,
   22, 179, 22,  0,
   51, 255,104,  0,
   85, 167, 22, 18,
  135, 100,  0,103,
  198,  16,  0,130,
  255,   0,  0,160};

// Gradient palette "rgi_15_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ds/rgi/tn/rgi_15.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte rgi_15_gp[] {
    0,   4,  1, 31,
   31,  55,  1, 16,
   63, 197,  3,  7,
   95,  59,  2, 17,
  127,   6,  2, 34,
  159,  39,  6, 33,
  191, 112, 13, 32,
  223,  56,  9, 35,
  255,  22,  6, 38};

extern const NSFastLED::TProgmemRGBGradientPalette_byte BlacK_Red_Magenta_Yellow_gp[] {
    0,   0,  0,  0,
   42,  42,  0,  0,
   84, 255,  0,  0,
  127, 255,  0, 45,
  170, 255,  0,255,
  212, 255, 55, 45,
  255, 255,255,  0};


// Gradient palette "Bad_Kitten_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/colo/Bionic_Blender/tn/Bad_Kitten.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 40 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte Bad_Kitten_gp[] {
    0,   1,  1,  1,
   86,   1,  1,  1,
   86,  28,  2, 23,
  102,  28,  2, 23,
  102,  97,  3, 13,
  117,  97,  3, 13,
  117, 177, 12,  2,
  132, 177, 12,  2,
  132, 206, 79,  1,
  255, 206, 79,  1};

// Gradient palette "Lucy_in_the_Sky_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/colo/vredeling/tn/Lucy_in_the_Sky.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 40 bytes of program space.

extern const NSFastLED::TProgmemRGBGradientPalette_byte Lucy_in_the_Sky_gp [] {
    0, 175,  1, 44,
   51, 175,  1, 44,
   51, 194, 14, 35,
  102, 194, 14, 35,
  102, 213, 40, 27,
  153, 213, 40, 27,
  153, 234, 80, 21,
  204, 234, 80, 21,
  204, 255,136, 15,
  255, 255,136, 15};

// for effects that are palette based
NSFastLED::CRGBPalette16 palettes[] = {
  Disorient_gp,
  Lucy_in_the_Sky_gp,
  Bad_Kitten_gp,
  es_pinksplash_08_gp,
  BlacK_Red_Magenta_Yellow_gp,
  es_pinksplash_07_gp,
  Sunset_Real_gp,
  rgi_15_gp,
  bhw1_01_gp,
  bhw1_04_gp,
  bhw3_50_gp,
  bhw3_52_gp,
  bhw3_02_gp,
  NSFastLED::LavaColors_p,
};
#define PALETTES_COUNT (sizeof(palettes)/sizeof(*palettes))

#endif
