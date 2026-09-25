//este programa funciona, pero el modulo usado es el v4.0 y requiere 5v, la wio tracker 1110 solo maneja 3.3v
//si el lcd es alimentado de manera externa, puede funcionar.
#include <Adafruit_TinyUSB.h>
#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

void setup() 
{
    lcd.begin(16, 2);
    lcd.setRGB(colorR, colorG, colorB);
    lcd.print("hello, world!");
    delay(1000);
}
void loop() 
{
    lcd.setCursor(0, 1);
    lcd.print(millis()/1000);
    delay(100);
}
