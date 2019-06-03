
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void fadePointerArrayToBlackBy(int number) {
  for (int addr = 0; addr < NUM_LEDS; addr++) {
    leds[addr]->fadeToBlackBy(number);
  }
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadePointerArrayToBlackBy(10);
  int pos = random16(NUM_LEDS);
  *leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadePointerArrayToBlackBy(20);
  int pos = beatsin16( 13, 0, NUM_LEDS - 1 );
  *leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) { //9948
    *leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadePointerArrayToBlackBy(20);
  byte dothue = 0;
  for ( int i = 0; i < 8; i++) {
    *leds[beatsin16( i + 7, 0, NUM_LEDS - 1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

SimplePatternList gPatterns = { confetti, sinelon, juggle, bpm };
String gPatternNames[] = { "confetti", "sinelon", "juggle", "bpm" };

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}
