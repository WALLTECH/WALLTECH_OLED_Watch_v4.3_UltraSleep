/*
 Draw radom pixel.
 
 Circuit:
 * I2C OLED Panel attached to pins SDA, SCL
 * Power inputs attached to pins 3V3, +5V,GND,GND,VIN
 
 Created 17 Jan 2012
 by: Jimbo.we(www.geekonfire.com)
 
 */
#include <Wire.h>
#include <GOFi2cOLED.h>

GOFi2cOLED GOFoled;

int x,y;

void setup()   {                  
  //default address is 0x3D.
  GOFoled.init(0x3D);
  // init done
  
  GOFoled.display(); // show splashscreen
  delay(2000);
  GOFoled.clearDisplay();
}


void loop() {
  
  x = random(0, GOFoled.width()-1); // 0 - 127
  y = random(0, GOFoled.height()-1);// 0 -63
  
  GOFoled.drawPixel( x, y, WHITE);  
  delay(200);
  GOFoled.display();
  GOFoled.clearDisplay();
}
