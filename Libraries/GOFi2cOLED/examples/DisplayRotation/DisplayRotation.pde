/*
 Rotate the content on the display.
 GOFoled.setRotation( value ); value can be 0-3.
 
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
 for(int i=0;i<4;i++)
 {
    GOFoled.clearDisplay();
    
    GOFoled.setRotation(i); //set rotation
    
    GOFoled.setTextSize(1);
    GOFoled.setTextColor(WHITE);
    GOFoled.setCursor(0,0);
    GOFoled.println("Hello, world!");
    GOFoled.println(-1234); 
    GOFoled.setTextColor(BLACK, WHITE); // 'inverted' text
    GOFoled.println(3.14159,5);
    GOFoled.setTextSize(2);
    GOFoled.setTextColor(WHITE);
    GOFoled.println("Rotation");
    GOFoled.display();
    delay(1000);
 }
  
}

