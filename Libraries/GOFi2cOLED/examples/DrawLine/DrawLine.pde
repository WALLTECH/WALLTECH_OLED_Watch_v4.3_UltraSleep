/*
 Line test.
 
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
  
  lineTest();
  
  GOFoled.clearDisplay();
  GOFoled.setTextSize(2);
  GOFoled.setTextColor(WHITE);  
  GOFoled.print("DONE");
  GOFoled.display(); // show splashscreen  
}

void loop() {
  
}

void lineTest() {  
  for (int i=0; i<GOFoled.width(); i+=4) {
    GOFoled.drawLine(0, 0, i, GOFoled.height()-1, WHITE);
    GOFoled.display();
  }
  for (int i=0; i<GOFoled.height(); i+=4) {
    GOFoled.drawLine(0, 0, GOFoled.width()-1, i, WHITE);
    GOFoled.display();
  }
  delay(250);
  
  GOFoled.clearDisplay();
  for (int i=0; i<GOFoled.width(); i+=4) {
    GOFoled.drawLine(0, GOFoled.height()-1, i, 0, WHITE);
    GOFoled.display();
  }
  for (int i=GOFoled.height()-1; i>=0; i-=4) {
    GOFoled.drawLine(0, GOFoled.height()-1, GOFoled.width()-1, i, WHITE);
    GOFoled.display();
  }
  delay(250);
  
  GOFoled.clearDisplay();
  for (int i=GOFoled.width()-1; i>=0; i-=4) {
    GOFoled.drawLine(GOFoled.width()-1, GOFoled.height()-1, i, 0, WHITE);
    GOFoled.display();
  }
  for (int i=GOFoled.height()-1; i>=0; i-=4) {
    GOFoled.drawLine(GOFoled.width()-1, GOFoled.height()-1, 0, i, WHITE);
    GOFoled.display();
  }
  delay(250);

  GOFoled.clearDisplay();
  for (int i=0; i<GOFoled.height(); i+=4) {
    GOFoled.drawLine(GOFoled.width()-1, 0, 0, i, WHITE);
    GOFoled.display();
  }
  for (int i=0; i<GOFoled.width(); i+=4) {
    GOFoled.drawLine(GOFoled.width()-1, 0, i, GOFoled.height()-1, WHITE); 
    GOFoled.display();
  }
  delay(250);
}
