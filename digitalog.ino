// created by Isaac Pierre Racine and Henrik Jonsson 
// may 2018 - 
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN  A3
#define NUMPIXELS  53
#define MIN_READING 101
#define MAX_READING 898
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
SoftwareSerial mySerial(A0, A1); // RX, TX
//const int PIXEL_PIN = A3; // NeoPixel input on analog 3 (A3).
const int POT_PIN = A2; // Potentiometer output on analog 2 (A2).
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(28, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() { //This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny84__)
  if (F_CPU == 8000000) clock_prescale_set(clock_div_1);
#endif
  mySerial.begin(9600);
  mySerial.println("smile");
  strip.begin();
  strip.show();
}
// loop is called every clock tick.
void loop() {
  int reading = analogRead(POT_PIN);
  //mySerial.println(reading);
  // Normalize the reading to [0, 1].
  float value = float(reading - MIN_READING) / float(MAX_READING - MIN_READING);
  mySerial.println(value);
  colorChase(value);  
}

// turn on LED ledActivo with color c.
void turnOnLed(int ledActivo, uint32_t c) {
  strip.setPixelColor(ledActivo, c);
  strip.show();
  //we need to first set it to the color we want and set it to black not to leave a trail afterwards.
  strip.setPixelColor(ledActivo, 0);
  strip.show();
}

// colorChase activates the appropriate LED, given value v.
void colorChase(float v) {
  // v is a value in [0, 1]
  // we want to subdivide the unit range in NUMPIXELS slots, and find the
  // index that v falls within, producing a number in [0, NUMPIXELS-1]
  int ledActivo = v * float(NUMPIXELS-1); 
  if (ledActivo < 0) {
    ledActivo = 0;
  } else if (ledActivo > NUMPIXELS-1) {
    ledActivo = NUMPIXELS-1;
  }
  // mySerial.println(ledActivo);
  int r = v * float(255);
  if (r < 0) {
    r = 0;
  } else if  (r> 255) {
    r = 255;
  }
  mySerial.println(r);
  uint32_t c = strip.Color(r, 255, 255);
  turnOnLed(ledActivo, c);
}

