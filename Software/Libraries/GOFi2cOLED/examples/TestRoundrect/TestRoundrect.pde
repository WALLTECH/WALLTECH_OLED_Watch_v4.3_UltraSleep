/*
 Round rectangle test.
 
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
  
  drawroundrectTest();
  delay(1000);
  GOFoled.clearDisplay();
   
  fillroundrectTest();
  delay(1000);
  GOFoled.clearDisplay();  
 
}

void drawroundrectTest(void) {
  for (int i=0; i<GOFoled.height()/2-2; i+=4) {
    GOFoled.drawRoundRect(i, i, GOFoled.width()-2*i, GOFoled.height()-2*i, GOFoled.height()/4, WHITE);
    GOFoled.display();
    delay(50);
  }
}

void fillroundrectTest(void) {
  boolean color = WHITE;
  for (int i=0; i<GOFoled.height()/2-2; i+=2) {
    GOFoled.fillRoundRect(i, i, GOFoled.width()-2*i, GOFoled.height()-2*i, GOFoled.height()/4, color);
    GOFoled.display();
    color = ~color;
    delay(50);    
  }
}
