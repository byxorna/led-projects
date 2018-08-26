
struct DeckSettings {
  uint8_t label;
  float crossfadePositionActive;
  uint8_t pattern;
  uint8_t palette;
  uint8_t animationIndex;
  CRGBPalette16 currentPalette; // current color palette
  unsigned long tPatternStart;  // time last pattern changed
  unsigned long tPaletteStart;  // time last palette changed
} ;
