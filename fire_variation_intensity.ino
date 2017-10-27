#include <SimplexNoise.h>
SimplexNoise sn;

#include <Adafruit_NeoPixel.h>

// the data pin for the NeoPixels
#define PIN 6

// How many NeoPixels we will be using, charge accordingly
#define NB_PIXELS 20

// Instatiate the NeoPixel from the ibrary
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NB_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

//VAR_INTENSITY must >= to BASE_INTENSITY, a check will be added later
#define BASE_INTENSITY 30
#define VAR_INTENSITY 30

//Value must be between 0 & 1.
//If you never want a LED to be completly off, put 0.1 to min
#define MIN_INTENSITY 0.1
#define MAX_INTENSITY 1.0

//Speed for variations, higher is slower
#define NOISE_SPEED_COLOR 2500
#define NOISE_SPEED_INTENSITY 1500

double n;
double n2;
double n3;

void setup() {
  strip.begin();  // initialize the strip
  strip.show();   // make sure it is visible
  strip.clear();  // Initialize all pixels to 'off'
  strip.setBrightness(60);
  //Serial.begin(9600);
}

void loop() {
  renderLEDs();
}

void renderLEDs() {

  unsigned int time = millis();

  for (int i = 0; i < NB_PIXELS; i++) {
    //adjust the mult and divide to change the global effect
    n = sn.noise(i << 3, (((float)time / (float)NOISE_SPEED_COLOR)));
    n2 = sn.noise((float)(i >> 3), (((float)time / (float)NOISE_SPEED_COLOR)));
    //used for variation intensity
    n3 = sn.noise(((float)time / (float)NOISE_SPEED_INTENSITY), i);
    n = (n + 1.0) / 2;
    n2 = (n2 + 1.0) / 2;

    //You can change the easing function here
    float v = SineEaseOut(n * n2);
    v = (MAX_INTENSITY - MIN_INTENSITY)*v + MIN_INTENSITY;
    float g = (BASE_INTENSITY + (n3 * VAR_INTENSITY)) * v;
    if (g < 0)
      g = 0;
    if (g > 255)
      g = 255;
    strip.setPixelColor(i, 255 * v , g , 0);
  }
  strip.show();

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
