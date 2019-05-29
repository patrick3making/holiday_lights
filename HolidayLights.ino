#include <FastLED.h>

FASTLED_USING_NAMESPACE

/*
 * Pin definitions
 */
#define THING_BOARD_BUTTON  0
#define THING_BOARD_LED     5
#define PSU_CONTROL_PIN     13
#define DATA_PIN_FRONT      18
#define DATA_PIN_SIDE       23
#define IIC_CLK             22
#define IIC_DATA            21

/*
 * Precompiler settings
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
 * Global Variables
 */
CRGB leds_front[NUM_LEDS_FRONT];
CRGB leds_side[NUM_LEDS_SIDE];
CRGB *leds[NUM_LEDS];

/*
 * Include other files
 */
#include "patterns.h"
#include "Leds.h"
#include "Wifi.h"
#include "web_gui.h"
#include "web_socket.h"


void setup() {
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Holiday Lights");
#endif

  pinMode(THING_BOARD_LED, OUTPUT);
  pinMode(THING_BOARD_BUTTON, INPUT_PULLUP);  
  pinMode(PSU_CONTROL_PIN, OUTPUT);

  setupWifi(); // some second delay for recovery

  setupLeds();

  setupWebPage();

  setupWebSocket();
}

void powerSupplyOn(){
  digitalWrite(PSU_CONTROL_PIN, LOW);
  
#ifdef DEBUG
  Serial.println("PSU On");
#endif
}

void powerSupplyOff(){
  digitalWrite(PSU_CONTROL_PIN, HIGH);
  
#ifdef DEBUG
  Serial.println("PSU Off");
#endif
}

void loop()
{
  ledsLoop();  
  webSocketLoop();
  webServerLoop();
}
