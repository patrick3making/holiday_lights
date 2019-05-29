
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

void setupLeds(){
  fillLedArray();
#ifdef DEBUG
  Serial.print("Setup LEDs");
#endif
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN_FRONT, COLOR_ORDER>(leds_front, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN_SIDE, COLOR_ORDER>(leds_side, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
#ifdef DEBUG
  Serial.print("  set brightness: ");
  Serial.println(BRIGHTNESS);
  Serial.println("complete");
#endif
}

void ledsLoop(){
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
