/* custom color palettes */
// orange 255,102,0 FF6600
// pink 255,0,255 #ff00ff
// pornj 255,51,51 #ff3333
DEFINE_GRADIENT_PALETTE( Disorient_gp ) {
  0,   0,   0,   0,    // black
  75, 255,  26, 153,    // pink
  147, 255,  51,  51,    // pornj
  208, 255, 111,  15,    // orange
  255, 255, 255, 255,
}; // white

DEFINE_GRADIENT_PALETTE( es_pinksplash_08_gp ) {
  0, 126, 11, 255,
  127, 197,  1, 22,
  175, 210, 157, 172,
  221, 157,  3, 112,
  255, 157,  3, 112
};

DEFINE_GRADIENT_PALETTE( es_pinksplash_07_gp ) {
  0, 229,  1,  1,
  61, 242,  4, 63,
  101, 255, 12, 255,
  127, 249, 81, 252,
  153, 255, 11, 235,
  193, 244,  5, 68,
  255, 232,  1,  5
};

DEFINE_GRADIENT_PALETTE( Sunset_Real_gp ) {
  0, 120,  0,  0,
  22, 179, 22,  0,
  51, 255, 104,  0,
  85, 167, 22, 18,
  135, 100,  0, 103,
  198,  16,  0, 130,
  255,   0,  0, 160
};

DEFINE_GRADIENT_PALETTE( rgi_15_gp ) {
  0,   4,  1, 31,
  31,  55,  1, 16,
  63, 197,  3,  7,
  95,  59,  2, 17,
  127,   6,  2, 34,
  159,  39,  6, 33,
  191, 112, 13, 32,
  223,  56,  9, 35,
  255,  22,  6, 38
};

DEFINE_GRADIENT_PALETTE( BlacK_Red_Magenta_Yellow_gp ) {
  0,   0,  0,  0,
  42,  42,  0,  0,
  84, 255,  0,  0,
  127, 255,  0, 45,
  170, 255,  0, 255,
  212, 255, 55, 45,
  255, 255, 255,  0
};



// Gradient palette "Lucy_in_the_Sky_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/colo/vredeling/tn/Lucy_in_the_Sky.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 40 bytes of program space.

DEFINE_GRADIENT_PALETTE( Lucy_in_the_Sky_gp ) {
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
CRGBPalette16 palettes[] = {
  Disorient_gp,
  Lucy_in_the_Sky_gp,  Lucy_in_the_Sky_gp,
  Lucy_in_the_Sky_gp,
  es_pinksplash_07_gp,
  Sunset_Real_gp,
  rgi_15_gp,
  BlacK_Red_Magenta_Yellow_gp,
  es_pinksplash_08_gp,
  LavaColors_p,
};
#define PALETTES_COUNT (sizeof(palettes)/sizeof(*palettes))
