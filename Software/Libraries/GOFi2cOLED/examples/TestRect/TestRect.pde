/*
 Rectangle test.
 
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
  
  rectTest();
  delay(1000);
  GOFoled.clearDisplay();
   
  fillrectTest();
  delay(1000);
  GOFoled.clearDisplay();  
 
}

void rectTest(void) {
  for (int i=0; i<GOFoled.height()/2; i+=4) {
    GOFoled.drawRect(i, i, GOFoled.width()-2*i, GOFoled.height()-2*i, WHITE);
    GOFoled.display();
    delay(100);
  }
}

void fillrectTest(void) {
  boolean color = WHITE;
  for (int i=0; i<GOFoled.height()/2; i+=3) {
    // alternate colors
    GOFoled.fillRect(i, i, GOFoled.width()-i*2, GOFoled.height()-i*2, color);
    GOFoled.display();
    color = ~color;
    delay(100);    
  }
}
