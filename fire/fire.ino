/*
  Fire Effect
  This project is a simple code base to create a fire/lava effect with an Arduino board and a WS2812B compatible LED-strip. 
  For example a NeoPixel(https://www.adafruit.com/category/168)
  The circuit:
  * Arduino Board
  * WS2812B compatible LED-strip
  Created 27/10/2017
  By Brice Culas - CosplayEnrichmentCenter
  Modified 06/11/2017
  By Brice Culas - CosplayEnrichmentCenter
  https://github.com/CosplayEnrichmentCenter/Arduino-FireEffect
*/

#include <FastLED.h>

/********BASIC SETTINGS********/

// the data pin for the NeoPixels
#define DATA_PIN 6

// How many NeoPixels we will be using, charge accordingly
#define NUM_LEDS 60

//The variation in yellow color to create the fire effect, define the interval where the color can change.
#define MIN_VARIATION 0
#define MAX_VARIATION 30

//Value must be between 0 & 1.
//If you never want a LED to be completly off, put 0.1 to min
#define MIN_INTENSITY 0.1
#define MAX_INTENSITY 1.0

//Speed for variations, higher is slower
#define NOISE_SPEED_COLOR 5
#define NOISE_SPEED_INTENSITY 3

/******************CODE*****************/
/**************DO NOT TOUCH*************/
/*********unless you really need********/

double n;
double ni;

const byte RED = 255;

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  //strip.setBrightness(60);
  //Serial.begin(9600);

}

void loop() {
  renderLEDs();
}

unsigned int lastTime = 0;
void renderLEDs() {

  unsigned int time = millis();

  //Serial.println(1000/(time - lastTime));
  lastTime = time;

  for (int i = 0; i < NUM_LEDS; i++) {
    //adjust the mult and divide to change the global effect
    // will be added to advanced settings later
    n = inoise8(i*250 , (time+i)/NOISE_SPEED_COLOR);

    ni = inoise8(i*500 , (time+i)/NOISE_SPEED_INTENSITY);

    //You can change the easing function here
    //Used to avoid a linear effect and give a more natural curve.
    float v = QuadraticEaseInOut(n/255);
    float vi = QuadraticEaseInOut(ni/255);
    
    vi = (MAX_INTENSITY - MIN_INTENSITY) * v + MIN_INTENSITY;
    float red = vi *(RED*v);
    float yellow = vi *((MAX_VARIATION - MIN_VARIATION)*v + MIN_VARIATION);

    leds[i] = CRGB(red , yellow , 0);
  }
  FastLED.show();

}

float CubicEaseInOut(float p)
{
  if (p < 0.5)
  {
    return 4 * p * p * p;
  }
  else
  {
    float f = ((2 * p) - 2);
    return 0.5 * f * f * f + 1;
  }
}

float QuadraticEaseInOut(float p)
{
  if (p < 0.5)
  {
    return 2 * p * p;
  }
  else
  {
    return (-2 * p * p) + (4 * p) - 1;
  }
}

float SineEaseOut(float p)
{
  return sin(p * M_PI_2);
}
