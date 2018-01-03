//This is a simple voltmeter is based on a pair of adafruit breakout boards:
//  7-segment display w/ I2C backpack https://www.adafruit.com/product/880
//  ADS1115 16-bit ADC https://www.adafruit.com/product/1085
//and a Feather Huzzah, but my preferred arduino dev-platform is the
//Sparkfun redboard: https://www.sparkfun.com/products/13975
//
//The sketch also uses a button, but you can make do by shorting the 3.3V
//output to pin 2 via a 10K resistor. Since I haven't gotten around to
//building the debutton circuit, that's exactly what I'm currently doing
//
//To use: connect input 0 of the ADC to the low side of the potential change
//to be measured, and input 1 on the high side. You can fine tune the output
//by changing the value of averageSize, but since it is #defined, it can only
//be unsigned int. Further tuning is possible by making it a float or a double.

#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#define averageSize 8
#define LED 13
#define buttonPin 2

Adafruit_7segment matrix = Adafruit_7segment();

Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
//Adafruit_ADS1015 ads;     /* Use thi for the 12-bit version */

boolean report = false;
unsigned long adc0 = 0;
int i = 0;
volatile int buttonState = 0;         // variable for reading the pushbutton status

void setup(void)
{
  matrix.begin(0x70);
  ads.begin();
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  matrix.clear();
  matrix.writeDisplay();
}

void loop()
{
  if ((report == true) && (i < 1024))
  {
    analogWrite(11, 128);
    detachInterrupt(0);
    digitalWrite(LED, HIGH);
    adc0 += ads.readADC_Differential_0_1();
    i++;
    if (i % averageSize == 0)
    {
      matrix.print(12.5 * (float)adc0 / (averageSize * pow(2, 16)));
      matrix.writeDisplay();
      adc0 = 0;
    }
  }
  else
  {
    analogWrite(11,0);
    matrix.clear();
    matrix.writeDisplay();
    digitalWrite(LED, LOW);
    report = false;
    attachInterrupt(0, pin_ISR, CHANGE);
  }
}

void pin_ISR() {
  report = true;
  i = 0;
}
