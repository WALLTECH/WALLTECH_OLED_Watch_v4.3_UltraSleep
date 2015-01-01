/*
 Brightness Control
 
 Brightness Step:0-255
 
 Circuit:
 * I2C OLED Panel attached to pins SDA, SCL
 * Power inputs attached to pins 3V3, +5V,GND,GND,VIN
 
 Created 17 Jan 2012
 by: Jimbo.we(www.geekonfire.com)
 
 */

#include <Wire.h>
#include <GOFi2cOLED.h>

GOFi2cOLED GOFoled;

int Brightness = 0;

void setup()   {                  
  //default address is 0x3D.
  GOFoled.init(0x3D);
  // init done
  
  GOFoled.display(); // show splashscreen
  GOFoled.setTextSize(1);
  GOFoled.setTextColor(WHITE);  
}


void loop() {
  
  GOFoled.setBrightness(Brightness++);  // Change the brightness 0 - 255
  GOFoled.setCursor(56,56);              // Set the Cursor position 
  GOFoled.print("Bright:"); 
  GOFoled.setCursor(104,56);              // Set the Cursor position
  GOFoled.clearArea(104, 56,24, 8);  // clearArea(x,y,w,h),clear the area for nunber.
  GOFoled.setCursor(104,56);              // Set the Cursor position  
  GOFoled.print(Brightness);
  GOFoled.display();  
  if(Brightness >= 255)
  {
      GOFoled.setCursor(0,56);              // Set the Cursor position  .
      GOFoled.print("Max:255");
      GOFoled.display();
      while(1);
  }
}
