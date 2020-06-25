#ifndef _PALETTES_H_
#define _PALETTES_H_

#ifndef _FASTLED_H_
#include "FastLED.h"
#endif



// Gradient palette "bhw1_purplered_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_purplered.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.
DEFINE_GRADIENT_PALETTE(bhw1_purplered_gp ) {
    0, 255,  0,  0,
  255, 107,  1,205};

// Gradient palette "autumnrose_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rc/tn/autumnrose.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 32 bytes of program space.

DEFINE_GRADIENT_PALETTE( autumnrose_gp ) {
    0,  71,  3,  1,
   45, 128,  5,  2,
   84, 186, 11,  3,
  127, 215, 27,  8,
  153, 224, 69, 13,
  188, 229, 84,  6,
  226, 242,135, 17,
  255, 247,161, 79};


// Gradient palette "rainbow_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ds9/tn/rainbow.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.
DEFINE_GRADIENT_PALETTE( rainbow_gp ) {
    0, 255,  0,255,
   51,   0,  0,255,
  102,   0,255,255,
  153,   0,255,  0,
  204, 255,255,  0,
  255, 255,  0,  0};


// Gradient palette "bhw2_xc_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_xc.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw2_xc_gp ) {
    0,   4,  2,  9,
   58,  16,  0, 47,
  122,  24,  0, 16,
  158, 144,  9,  1,
  183, 179, 45,  1,
  219, 220,114,  2,
  255, 234,237,  1};



// for effects that are palette based
CRGBPalette16 palettes[] = {
  bhw1_purplered_gp,
  bhw2_xc_gp,
  autumnrose_gp,
  rainbow_gp,
};
#define PALETTES_COUNT (sizeof(palettes)/sizeof(*palettes))

#endif
