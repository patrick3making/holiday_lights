#include <FastLED.h>

FASTLED_USING_NAMESPACE

/*
   Pin definitions
*/
#define THING_BOARD_BUTTON  0
#define THING_BOARD_LED     5
#define PSU_CONTROL_PIN     13
#define DATA_PIN_RIGHT      23
#define DATA_PIN_LEFT       17
#define CLOCK_PIN_RIGHT     18
#define CLOCK_PIN_LEFT      16
#define IIC_CLK             22
#define IIC_DATA            21

/*
   Precompiler settings
*/
#define BRIGHTNESS          255
#define FRAMES_PER_SECOND   30
#define MAX_SPEED
#define LED_TYPE            WS2812
#define COLOR_ORDER         GRB
#define NUM_LEDS_FRONT      1200
#define NUM_LEDS_SIDE       600
#define NUM_LEDS (NUM_LEDS_SIDE + NUM_LEDS_FRONT)
#define DEBUG

/*
   Global Variables
*/
CRGB leds_front[NUM_LEDS_FRONT];
CRGB leds_side[NUM_LEDS_SIDE];
CRGB *leds[NUM_LEDS];
bool powerOn = false;
long powerCycleTime;

/*
   Include other files
*/
#include "patterns.h"
#include "Leds.h"
#include "Wifi.h"
#include "web_server.h"
#include "web_socket.h"

void powerSupplyOn() {
  if (digitalRead(PSU_CONTROL_PIN) == HIGH) {
    digitalWrite(PSU_CONTROL_PIN, LOW);
    powerCycleTime = millis();

#ifdef DEBUG
    Serial.println("PSU On");
#endif
  }
}

void powerSupplyOff() {
  if (digitalRead(PSU_CONTROL_PIN) == LOW) {
    digitalWrite(PSU_CONTROL_PIN, HIGH);
    powerCycleTime = millis();

#ifdef DEBUG
    Serial.println("PSU Off");
#endif
  }
}

void setPsu() {
  // don't toggle power within less than 3 seconds
  if ( millis() - powerCycleTime > 3000) {
    if (digitalRead(THING_BOARD_BUTTON) == LOW) {
      //delay(10);
      powerOn = !powerOn;
      powerCycleTime = millis();
#ifdef DEBUG
      Serial.print("Button press Power State: ");
      Serial.println(powerOn);
#endif
    }
    if (powerOn) {
      powerSupplyOn();
    }
    else {
      powerSupplyOff();
    }
  }
}

void setup() {
  powerCycleTime = millis();
  Serial.begin(9600);
  Serial.println("Holiday Lights");

  pinMode(THING_BOARD_LED, OUTPUT);
  pinMode(THING_BOARD_BUTTON, INPUT_PULLUP);
  pinMode(PSU_CONTROL_PIN, OUTPUT);

  powerSupplyOff();

  setupWifi(); // some delay for WS2812 recovery

  setupWebServer();

  //setupWebSocket();

  setupLeds();
}

void loop()
{
  ledsLoop(4);
  //webSocketLoop();
  webServerLoop();
  setPsu();
}
