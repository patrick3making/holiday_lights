void fillLedArray(){
#ifdef LEDS_DEBUG
  Serial.println("  fill array with pointers...");
#endif
  int addr = 0;
  for (int left = NUM_LEDS_LEFT - 1; left >= 0; left--)
  {
    leds[addr] = &leds_left[left];
    addr++;
  }

#ifdef LEDS_DEBUG
  Serial.print("Left: ");
  Serial.println(addr);
#endif
  for (int right = 0; right < NUM_LEDS_RIGHT; right++)
  {
    leds[addr] = &leds_right[right];
    addr++;
  }
#ifdef LEDS_DEBUG
  Serial.print("Right: ");
  Serial.println(addr);
  Serial.println("done");
#endif
}

void setupLeds()
{
#ifdef LEDS_DEBUG
  Serial.print("Setup LEDs");
#endif
  fillLedArray();
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN_RIGHT, COLOR_ORDER>(leds_right, NUM_LEDS_RIGHT).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN_LEFT, COLOR_ORDER>(leds_left, NUM_LEDS_LEFT).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
#ifdef LEDS_DEBUG
  Serial.println("");
  Serial.print("  set brightness: ");
  Serial.println(BRIGHTNESS);
  Serial.println("complete");
#endif
}

void ledsLoop(uint8_t singlePattern = 255)
{
  if (singlePattern == 255)
  {
    gPatterns[gCurrentPatternNumber]();
  }
  else
  {
    gPatterns[singlePattern]();
  }

  // send the 'leds' array out to the actual LED strip
  FastLED.show();

#ifndef MAX_SPEED
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);
#endif

  // do some periodic updates
  EVERY_N_MILLISECONDS(20)
  {
    gHue++; // slowly cycle the "base color" through the rainbow
  }

}
