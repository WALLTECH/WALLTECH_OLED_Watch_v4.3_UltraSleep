/*
 Triangle test.
 
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
  
  drawtriangleTest();
  delay(1000);
  GOFoled.clearDisplay();
   
  filltriangleTest();
  delay(1000);
  GOFoled.clearDisplay();  
 
}

void drawtriangleTest(void) {
  for (int i=0; i<min(GOFoled.width(),GOFoled.height())/2; i+=5) {
    GOFoled.drawTriangle(GOFoled.width()/2, GOFoled.height()/2-i,
                     GOFoled.width()/2-i, GOFoled.height()/2+i,
                     GOFoled.width()/2+i, GOFoled.height()/2+i, WHITE);
    GOFoled.display();
    delay(500);
  }
}

void filltriangleTest(void) {
  boolean color = WHITE;
  for (int i=min(GOFoled.width(),GOFoled.height())/2; i>0; i-=5) {
    GOFoled.fillTriangle(GOFoled.width()/2, GOFoled.height()/2-i,
                     GOFoled.width()/2-i, GOFoled.height()/2+i,
                     GOFoled.width()/2+i, GOFoled.height()/2+i, color);
    color =~color;
    GOFoled.display();
    delay(500);
  }
}
