/*
 Circle test.
 
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
  
}

void loop() {
  
  drawcircleTest();
  GOFoled.clearDisplay();
  delay(2000);  
  fillcircleTest();
  GOFoled.clearDisplay();
  delay(2000);  
}

void drawcircleTest(void) {
  for (int i=0; i<GOFoled.height(); i+=2) {
    GOFoled.drawCircle(GOFoled.width()/2, GOFoled.height()/2, i, WHITE);
    GOFoled.display();
  }
}

void fillcircleTest(void) {
  boolean color = WHITE;
  int r = GOFoled.height()/2;   
  while( r>0)
  {
//    GOFoled.clearDisplay();    
    GOFoled.fillCircle(GOFoled.width()/2, GOFoled.height()/2, r, color);
    GOFoled.display();
    r -= 4;
    color=~color;
    delay(1000);
  }
}
