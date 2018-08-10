

DEFINE_GRADIENT_PALETTE( Sunset_Real_gp ) {
  0, 120,  0,  0,
  22, 179, 22,  0,
  51, 255, 104,  0,
  85, 167, 22, 18,
  135, 100,  0, 103,
  198,  16,  0, 130,
  255,   0,  0, 160
};

// Gradient palette "bhw2_39_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_39.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw2_39_gp ) {
    0,   2,184,188,
   33,  56, 27,162,
   66,  56, 27,162,
  122, 255,255, 45,
  150, 227, 65,  6,
  201,  67, 13, 27,
  255,  16,  1, 53};


// Gradient palette "bhw2_39a_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_39a.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw2_39a_gp ) {
    0,   2,184,188,
   47,  17, 87,176,
   96,  56, 27,162,
  106,  56, 27,162,
  147,  56, 27,162,
  190, 133,109, 93,
  255, 255,255, 45};

// Gradient palette "bhw4_009_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_009.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 32 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw4_009_gp ) {
    0,  66,186,192,
   43,   1, 22, 71,
   79,   2,104,142,
  117,  66,186,192,
  147,   2,104,142,
  186,   1, 22, 71,
  224,   2,104,142,
  255,   4, 27, 28};

// Gradient palette "bhw4_051_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_051.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw4_051_gp ) {
    0,   1,  1,  4,
   28,  16, 24, 77,
   66,  35, 87,160,
  101, 125,187,205,
  127, 255,233, 13,
  145, 125,187,205,
  193,  28, 70,144,
  224,  14, 19, 62,
  255,   1,  1,  4};

// Gradient palette "b_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ds9/tn/b.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( b_gp ) {
    0,   0,  0,  0,
   63,   0,  0,255,
  127, 255,  0,  0,
  191, 255,255,  0,
  255, 255,255,255};

// Gradient palette "bhw2_48_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_48.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw2_48_gp ) {
    0,   0,  0,  8,
   51,  20, 40,147,
   61,  15,117,115,
  109,  26,  3,186,
  163,  78, 43,205,
  211, 199, 17, 84,
  255,  27,  1, 10};

// Gradient palette "bhw2_50_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_50.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw2_50_gp ) {
    0,   8,  2, 23,
   84,  47,  7,102,
  138,  16, 46,147,
  173,   2,127,203,
  255,   1,  7, 11};



// for effects that are palette based
CRGBPalette16 palettes[] = {
  bhw2_39_gp,
  bhw4_051_gp,
  bhw2_50_gp,
  bhw4_009_gp,
  bhw2_48_gp,
  bhw2_39a_gp,
  b_gp,
};
#define PALETTES_COUNT (sizeof(palettes)/sizeof(*palettes))
