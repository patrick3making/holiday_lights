#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define DEBUG

#define MAX_SPEED
#define DATA_PIN_FRONT    32
#define DATA_PIN_SIDE    33
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS_FRONT    300
CRGB leds_front[NUM_LEDS_FRONT];
#define NUM_LEDS_SIDE    300
CRGB leds_side[NUM_LEDS_SIDE];
#define NUM_LEDS (NUM_LEDS_SIDE + NUM_LEDS_FRONT)
CRGB *leds[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  30

void fillLedArray() {
#ifdef DEBUG
  Serial.print("  fill array with pointers...");
#endif
  int addr = 0;
  for ( int side = NUM_LEDS_SIDE - 1; side >= 0 ; side--) {
    leds[addr] = &leds_side[side];
    addr++;
#ifdef DEBUG
    Serial.print(".");
#endif
  }
  for ( int front = 0; front < NUM_LEDS_FRONT ; front++) {
    leds[addr] = &leds_front[front];
    addr++;
#ifdef DEBUG
    Serial.print(",");
#endif
  }
#ifdef DEBUG
  Serial.println("done");
#endif
}

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Holiday Lights");
#endif

  delay(3000); // 3 second delay for recovery

  fillLedArray();
#ifdef DEBUG
  Serial.print("  setup...");
#endif
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN_FRONT, COLOR_ORDER>(leds_front, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN_SIDE, COLOR_ORDER>(leds_side, NUM_LEDS).setCorrection(TypicalLEDStrip);
#ifdef DEBUG
  Serial.println("complete");
#endif

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
#ifdef DEBUG
  Serial.print("  set brightness: ");
  Serial.println(BRIGHTNESS);
#endif
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { confetti, sinelon, juggle, bpm };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();

#ifndef MAX_SPEED
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);
#endif

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }
  EVERY_N_SECONDS( 10 ) {
    nextPattern();  // change patterns periodically
  }
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
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

void fadePointerArrayToBlackBy(int number) {
  for (int addr = 0; addr < NUM_LEDS; addr++) {
    leds[addr]->fadeToBlackBy(number);
  }
}

