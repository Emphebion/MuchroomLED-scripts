#include "FastLED.h"


#define NUM_LEDS 72
#define LED_TYPE NEOPIXEL
#define BRIGHT 64
#define COLOR_ORDER GRB
#define LED_PIN 2
#define POW_PIN 3
#define MAX_GLOWS 8
#define NR_COLORS 12

int index[MAX_GLOWS];
CRGB _leds[NUM_LEDS];
CRGB _colors[NR_COLORS] = {CRGB::Chartreuse,CRGB::DarkGoldenrod,CRGB::DarkGreen,CRGB::DarkOliveGreen,CRGB::Goldenrod,CRGB::GreenYellow,CRGB::Khaki,CRGB::LemonChiffon,CRGB::LimeGreen,CRGB::Tan,CRGB::LightSalmon,CRGB::DarkOrange};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("TEST");
  pinMode(POW_PIN,OUTPUT);
  digitalWrite(POW_PIN,HIGH);
  FastLED.addLeds<LED_TYPE, LED_PIN>(_leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHT);
  randomSeed(analogRead(0));
}

void loop() {
  // put your main code here, to run repeatedly:
  setRandomIndex();
  setRandomColors();
  FastLED.show();
  glow();
  FastLED.delay(4000);
  fade();
  resetIndex();
}

void setRandomIndex(){
  int rnd = random(3,9);  
  for (int i = 0; i < MAX_GLOWS; i++){
    if (i < rnd) {
      index[i] = random(2,NUM_LEDS-1);
      for (int j = 0; j < i; j++){
        if (index[i] == index[j] || index[i]-1 == index[j] || index[i]+1 == index[j]){
          index[i] = 100;
          rnd++;
        }
      }
    }
    else
      index[i] = 100;
  }

}

void setRandomColors(){
  for (int i = 0; i < MAX_GLOWS; i++){
    CRGB color = _colors[random(1,NR_COLORS)];
    if (index[i] != 100){
      _leds[index[i]-1] = color;
      _leds[index[i]-1].fadeToBlackBy(240);
      _leds[index[i]] = color;
      _leds[index[i]].fadeToBlackBy(240);
      _leds[index[i]+1] = color;
      _leds[index[i]+1].fadeToBlackBy(240);
    }
  }
}

void glow(){
  int glowValue = 13;
  unsigned long startTime = millis();
  while ((millis() - startTime) < 1000){
    for (int i = 0; i < MAX_GLOWS; i++){
      if (index[i] != 100){
        _leds[index[i]].maximizeBrightness(glowValue);
        _leds[index[i]-1] = _leds[index[i]];
        _leds[index[i]+1] = _leds[index[i]];
      }
    }
    FastLED.show();
    FastLED.delay(50);
    if (glowValue >= 250){
      glowValue = 250;
    }
    else {
      glowValue += 13;
    }
  }
}

void fade(){
  unsigned long startTime = millis();
  while ((millis() - startTime) < 1000){
    for (int i = 0; i < MAX_GLOWS; i++){
      if (index[i] != 100){
        _leds[index[i]].fadeToBlackBy(20);
        _leds[index[i]-1] = _leds[index[i]];
        _leds[index[i]+1] = _leds[index[i]];
      }
    }
    FastLED.show();
    FastLED.delay(50);
  }
  for (int i = 0; i < MAX_GLOWS; i++){
    _leds[index[i]] = CRGB::Black;
    _leds[index[i]-1] = CRGB::Black;
    _leds[index[i]+1] = CRGB::Black;
  }
}

void resetIndex(){
  for (int i = 0; i < MAX_GLOWS; i++){
    index[i] = 0;
  }
}