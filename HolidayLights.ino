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

#define STATUS_LED 5

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  30

#include "patterns.h"
#include "web_socket.h"
#include "web_gui.h"

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


  pinMode(STATUS_LED, OUTPUT);

  setupWifi(); // some second delay for recovery

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

  setupWebPage();

  setupWebSocket();
}

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
  
  webSocketLoop();
  webServerLoop();
}
