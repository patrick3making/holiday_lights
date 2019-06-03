#include <FastLED.h>

FASTLED_USING_NAMESPACE

/*
   Pin definitions
*/
#define THING_BOARD_BUTTON  0
#define THING_BOARD_LED     5
#define PSU_CONTROL_PIN     13
#define DATA_PIN_FRONT      18
#define DATA_PIN_SIDE       23
#define IIC_CLK             22
#define IIC_DATA            21

/*
   Precompiler settings
*/
#define BRIGHTNESS          96
#define FRAMES_PER_SECOND   30
#define MAX_SPEED
#define LED_TYPE            WS2812
#define COLOR_ORDER         GRB
#define NUM_LEDS_FRONT      300
#define NUM_LEDS_SIDE       300
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
#include "web_gui.h"
#include "web_socket.h"


void setup() {
  powerCycleTime = millis();
  Serial.begin(115200);
  Serial.println("Holiday Lights");

  pinMode(THING_BOARD_LED, OUTPUT);
  pinMode(THING_BOARD_BUTTON, INPUT_PULLUP);
  pinMode(PSU_CONTROL_PIN, OUTPUT);

  setupWifi(); // some delay for WS2812 recovery

  setupWebPage();

  setupWebSocket();

  setupLeds();
}

void powerSupplyOn() {
  if (!powerOn) {
    digitalWrite(PSU_CONTROL_PIN, LOW);
    powerCycleTime = millis();

#ifdef DEBUG
    Serial.println("PSU On");
#endif
  }
}

void powerSupplyOff() {
  if (powerOn) {
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
    if (powerOn) {
      powerSupplyOn();
    }
    else {
      powerSupplyOff();
    }
  }
}

void loop()
{
  ledsLoop();
  webSocketLoop();
  webServerLoop();
  setPsu();
}
