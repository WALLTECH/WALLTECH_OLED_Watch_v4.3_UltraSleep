/*
 Print
 
 Just like Serial.print/println
 
 Circuit:
 * I2C OLED Panel attached to pins SDA, SCL
 * Power inputs attached to pins 3V3, +5V,GND,GND,VIN
 
 Created 17 Jan 2012
 by: Jimbo.we(www.geekonfire.com)
 
 */

#include <Wire.h>
#include <GOFi2cOLED.h>

GOFi2cOLED GOFoled;

void setup()
{
  // default slave address is 0x3D
  GOFoled.init(0x3D);  //initialze  OLED display
  
  GOFoled.display(); // show splashscreen
  delay(2000);
  GOFoled.clearDisplay();
  
  GOFoled.setTextSize(1);
  GOFoled.setTextColor(WHITE);
  GOFoled.setCursor(0,0);
  GOFoled.println("Hello, world!");
  GOFoled.println(-1234); 
  GOFoled.println(3.14159);  
  GOFoled.setTextColor(BLACK, WHITE); // 'inverted' text
  GOFoled.println(3.14159,5);
  GOFoled.setTextSize(2);
  GOFoled.setTextColor(WHITE);
  GOFoled.print("0x"); GOFoled.println(0xDEADBEEF, HEX);
  GOFoled.display();

}

void loop()
{
  
}
