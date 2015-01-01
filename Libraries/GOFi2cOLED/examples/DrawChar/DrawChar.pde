/*
 Print Alphabet
 
 Circuit:
 * I2C OLED Panel attached to pins SDA, SCL
 * Power inputs attached to pins 3V3, +5V,GND,GND,VIN
 
 Created 17 Jan 2012
 by: Jimbo.we(www.geekonfire.com)
 
 */
#include <Wire.h>
#include <GOFi2cOLED.h>

GOFi2cOLED GOFoled;

void setup()   {                  
  //default address is 0x3D.
  GOFoled.init(0x3D);
  // init done
  
  GOFoled.display(); // show splashscreen
  delay(2000);
  GOFoled.clearDisplay();
  
  GOFoled.setTextSize(1);
  GOFoled.setTextColor(WHITE);
  GOFoled.setCursor(0,0);

  for (char i='A'; i <= 'Z'; i++)
    {
      GOFoled.print(i);
      GOFoled.display();      
    }
  GOFoled.println();
  
  GOFoled.setTextSize(2);  
  GOFoled.setTextColor(BLACK, WHITE);  // 'inverted' text
  
  for (char i='a'; i <= 'z'; i++)
    {
      GOFoled.print(i);
      GOFoled.display();
    }
}

void loop() {
 
}

