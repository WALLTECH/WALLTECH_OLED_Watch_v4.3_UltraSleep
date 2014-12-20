/*
 Horizontal Scroll Application
 
 Scroll_Left or Scroll_Right
 
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
  
}

void loop() {
  
  GOFoled.setHorizontalScrollProperties(Scroll_Left,5,7,Scroll_5Frames);  //Set the properties of Horizontal Scroll,scroll area:page5-page7
  GOFoled.activateScroll();             // Activate Scroll
  delay(2000);
  GOFoled.setHorizontalScrollProperties(Scroll_Right,5,7,Scroll_5Frames);  //Set the properties of Horizontal Scroll,scroll area:page5-page7
  delay(2000);  
}

