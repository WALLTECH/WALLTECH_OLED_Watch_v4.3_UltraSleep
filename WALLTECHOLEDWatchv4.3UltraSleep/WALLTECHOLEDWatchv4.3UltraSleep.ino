/*****************************************************************************************
 *  ___       __   ________  ___       ___   _________  _______   ________  ___  ___     
 * |\  \     |\  \|\   __  \|\  \     |\  \ |\___   ___\\  ___ \ |\   ____\|\  \|\  \    
 * \ \  \    \ \  \ \  \|\  \ \  \    \ \  \\|___ \  \_\ \   __/|\ \  \___|\ \  \\\  \   
 *  \ \  \  __\ \  \ \   __  \ \  \    \ \  \    \ \  \ \ \  \_|/_\ \  \    \ \   __  \  
 *   \ \  \|\__\_\  \ \  \ \  \ \  \____\ \  \____\ \  \ \ \  \_|\ \ \  \____\ \  \ \  \ 
 *    \ \____________\ \__\ \__\ \_______\ \_______\ \__\ \ \_______\ \_______\ \__\ \__\
 *     \|____________|\|__|\|__|\|_______|\|_______|\|__|  \|_______|\|_______|\|__|\|__|
 * 
 * This is the default code for the Walltech OLED Watch v4.3
 * 
 * Designed specifically to work with the Walltech OLED Watch v4.3:
 * ----> http://wp.me/P4qDhv-fm
 * 
 * Walltech invests time and resources providing this open source code, 
 * please support Walltech and open-source hardware by donating any amount!
 * https://www.paypal.com/us/cgi-bin/webscr?cmd=_flow&SESSION=LrIWifBuf102Rn7Ang7qJLttsEWICxRtd9iYW6zG0OBBt_zN2z6srd7XQXa&dispatch=5885d80a13c0db1f8e263663d3faee8d6cdb53fcfca2b5941339e576d7e42259
 * 
 * Written by John Wall for Walltech.  
 * BSD license, all text above must be included in any redistribution
 * 
 * Creative Commons License
 * This work by Walltech Industries is licensed
 * under a Creative Commons Attribution-ShareAlike 4.0 International License.
 * Largely based on a work at http://www.arduino.cc
 *****************************************************************************************/
#include <avr/sleep.h>
#include <Wire.h>// Including the necessary libraries
#include <GOFi2cOLED.h>// This library can be found here - http://www.geekonfire.com/wiki/index.php?title=I2C_OLED_Panel(128x64)
#include "RTClib.h"//These are Adafruit libraries, support them at www.adafruit.com!
#include "Adafruit_MCP9808.h"
#include "bitmaps.h"

GOFi2cOLED oled;// instantiating constructors 
RTC_DS1307 RTC;
// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

boolean timeSet = false;
//************NOTE************ 
// After uploading, change the above line to false
// and re-upload. This is very 
// important because without it,
// the time will reset to the original set
// time every power cycle instead 
// of keeping with the current time.

byte PROGMEM lbtn = 4;// naming the button pins
byte PROGMEM cbtn = 2;
byte PROGMEM rbtn = A1;

byte timehour;// declaring all program variables
byte timeminute;
byte timesecond;
byte lastsecond;
int timemillisecond;

byte buttonState_l;
byte buttonState_c;
byte buttonState_r;

byte PROGMEM charge = 3;
byte PROGMEM full = A3;
byte brightness = 0;// how bright the charging LED is
byte fadeAmount = 5;// how many points to fade the charging LED by
byte ledstate = LOW;
int lipostat;//Charge status
byte percent;

float f;
byte temperature[128];
byte graphMax = 100;
byte graphMin = 50;
byte graphPosition = 0;
byte point;

int value = LOW;// previous value of the LED
int PROGMEM stopwatchblink = 1000;
int stopButtonState;
int laststopButtonState;// variable to store last button state
byte blinking;// condition for blinking - timer is timing
long previousMillis = 0;// variable to store last time LED was updated
long startTime;// start time for stop watch
long elapsedTime;// elapsed time for stop watch
long runningTime;
int fractional;// variable used to store fractional part of time
byte hours;
byte minutes;
byte seconds;
long previousmillisseconds;

byte setMinute;
byte setHour;
byte setDay = 1;
byte setMonth = 1;
byte setYear = 2014;
boolean set = false;
byte setSelect = 0;

int PROGMEM interval = 333;

byte face = 1;
byte PROGMEM facenum = 9;

byte brightnessLevel = 4;

long sleepMillis;
int wakeTime = 10000;//The time the watch stays awake before going into sleep mode in milliseconds

byte A = 1;
byte B = 1;
byte C = 1;
byte D = 1;

void setup() { 

  oled.init(0x3c);// initiate the screen at its address
  oled.clearDisplay();

  Wire.begin();// Beginning the library functions

  RTC.begin();

  tempsensor.begin();

  if(timeSet) RTC.adjust(DateTime(__DATE__, __TIME__));// Setting the DS1307 time

  pinMode(lbtn, INPUT_PULLUP);// set button pinmodes to input pulled high
  pinMode(cbtn, INPUT_PULLUP);
  pinMode(rbtn, INPUT_PULLUP);

  pinMode(A7, INPUT);// sense pins and output LEDs set
  pinMode(A2, INPUT_PULLUP);
  pinMode(charge, OUTPUT);
  pinMode(full, OUTPUT);

  attachInterrupt(0, wakeUpNow, LOW); // use interrupt 0 (pin 2) and run function
  // wakeUpNow when pin 2 gets LOW 

    oled.setRotation(2);// the screen is actually mounted upside down, so we flip it
  oled.setBrightness(255);// full brightness to start
  oled.drawBitmap(1, 25, walltech2 ,126, 15, WHITE);// logo and startup animation
  oled.display();
  delay(1000);
  oled.setCursor(7,57);
  oled.print(F("v4.3 John Wall 2014"));
  oled.display();
  delay(1000);
  oled.clearDisplay();

  sleepMillis = millis();
  checkBatteryLevel();
}

void loop() {

  DateTime now = RTC.now();// the time variables are redefined

  timehour   = now.hour();
  timeminute = now.minute();
  timesecond = now.second();
  if (timehour > 12) timehour = timehour - 12;// comment these two ifs to convert to 24 hour time (haven't tested that)
  if (now.hour() == 0)
  {
    timehour = 12;
  }

  if(now.second() % 2 == 0 && point == 0)
  {
    double temp = getTemp();
    if(temp != 24.89) 
    {
      temperature[graphPosition] = (byte)temp;
      graphPosition++;
      if(graphPosition>127)
      {
        graphPosition = 0;
        memset(temperature,-1,sizeof(temperature));
      }
      if((byte)getTemp() < graphMin && (byte)getTemp() > -1) graphMin = (byte)getTemp();
      if((byte)getTemp() > graphMax) graphMax = (byte)getTemp();

      point++;
    }
  }

  if (now.second() % 2 == 1) point = 0;

  lipostat = analogRead(A2);// the charge status of the battery is measured

  checkButtons();// the button states are checked again

  if(buttonState_r == LOW)// if the right button is pressed, advance the faces by one and reset the sleep counter
  {
    delay(100);
    face++; 
    sleepMillis = millis();
    checkBatteryLevel();
  }

  if(buttonState_c == LOW)// if the center button is pressed just reset the sleep counter
  {
    delay(100);
    sleepMillis = millis();
    checkBatteryLevel();
  }

  if(buttonState_l == LOW)// left scroll
  {
    delay(100);
    face--; 
    sleepMillis = millis();
    checkBatteryLevel();
  }

  if(face < 1)// loop the faces back up again
  {
    face = facenum;
  }
  if(face > facenum)
  {
    face = 1;
  }

  if (face == 1)// first face
  {
    if(timehour<10)
    {
      displayNum(18,18,timehour);// print the hour's number

        if(now.second() % 2 == 0)// flash the colon once a second
      {
        oled.drawBitmap(49, 23, fontcolon, 3, 21, WHITE);
      }

      else
      {
        oled.drawRect(49, 23, 3, 21, BLACK);
      }

        displayNum(56,18,timeminute/10);
        displayNum(87,18,timeminute % 10);
    }

    else
    {
      displayNum(0,18,timehour/10);
      displayNum(31,18,timehour % 10);

      if(now.second() % 2 == 0)
      {
        oled.drawBitmap(61, 23, fontcolon, 3, 21, WHITE);
      }

      else
      {
        oled.drawRect(61, 23, 3, 21, BLACK);
      }

        displayNum(69,18,timeminute/10);
        displayNum(100,18,timeminute % 10);
    }

    oled.setCursor(14,57);// set cursor and print the date
    oled.setTextSize(1);
    
    switch(now.dayOfWeek()){
      case 0:
       oled.print(F("Sun "));
       break;
      case 1:
       oled.print(F("Mon "));
       break;
      case 2:
       oled.print(F("Tue "));
       break;
      case 3:
       oled.print(F("Wed "));
       break;
      case 4:
       oled.print(F("Thu "));
       break;
      case 5:
       oled.print(F("Fri "));
       break;
      case 6:
       oled.print(F("Sat "));
       break;
    }

    oled.print(now.month());
    oled.print('/');
    oled.print(now.day());
    oled.print('/');    
    oled.print(now.year());  

    oled.print(' ');
    oled.print(percent);
    oled.print('%');
  }

  else if(face == 2)// printing each number of the time as the corresponding bitmap of the font
  {
    if(timehour < 10)
    {
      blindNum(25,0,timehour%10);
      blindNum(57,0,timeminute/10);
      blindNum(81,0,timeminute%10); 

      if(timesecond % 2 == 0) oled.drawBitmap(48, 0, blindcolon ,8, 64, WHITE);
    }

    else
    {
      blindNum(1,0,timehour/10);
      blindNum(25,0,timehour%10);
      blindNum(72,0,timeminute/10);
      blindNum(102,0,timeminute%10); 
      if(timesecond % 2 == 0) oled.drawBitmap(56, 0, blindcolon ,8, 64, WHITE);
    }
  }

  else if (face == 3)// optical illusion box font
  {
    if(timehour < 10)
    {
      displayBoxFont(15,0,timehour%10);
      displayBoxFont(53,0,timeminute/10);
      displayBoxFont(83,0,timeminute%10); 

      if(timesecond % 2 == 0) oled.drawBitmap(45, 0, boxcolon ,8, 64, WHITE);

      else oled.drawBitmap(45, 0, boxcoloninverted ,8, 64, WHITE);

    }

    else
    {
      displayBoxFont(0,0,timehour/10);
      displayBoxFont(30,0,timehour%10);
      displayBoxFont(68,0,timeminute/10);
      displayBoxFont(98,0,timeminute%10); 
      if(timesecond % 2 == 0) oled.drawBitmap(60, 0, boxcolon ,8, 64, WHITE);

      else oled.drawBitmap(60, 0, boxcoloninverted ,8, 64, WHITE);
    } 

  }

  else if(face == 4)// the bitmaps are text, same as before pretty much
  {
    oled.drawBitmap(0, 0, its, 72, 16, WHITE);

    if(timeminute == 0)
    {
      textNum(0,24,timehour);
      oled.drawBitmap(0, 48, oclock, 128, 16, WHITE);
    }

    else if(timeminute > 0 && timeminute < 10)
    {
      textNum(0,24,timehour);
      oled.drawBitmap(0, 48, oh, 72, 16, WHITE);
      textNum(24,48, timeminute);
    }

    else if(timeminute > 9 && timeminute < 20)
    {
      textNum(0,24,timehour);
      textNum(0,48,timeminute);
    }

    else if(timeminute >= 20 && timeminute% 10 == 0)
    {

      textNum(0,24, timehour);

      textNum(0, 48, timeminute);

    }

    else if(timeminute > 20 && timeminute% 10 != 0)
    {
      textNum(0,16,timehour);

      textNum(0, 32, (timeminute/10)*10);

      textNum(0,48, timeminute%10);

    }

  }

  else if(face == 5)// this stuff does the math necessary for the analog clock face
  {
    oled.drawRect( 0, 0,  128, 64, WHITE );
    oled.drawFastVLine( 62, 2, 4, WHITE );
    oled.drawFastVLine( 64, 2, 4, WHITE );
    oled.drawFastHLine( 120, 32, 6, WHITE );
    oled.drawFastVLine( 63, 58, 4, WHITE );
    oled.drawFastHLine( 2, 32, 6, WHITE );

    oled.drawPixel( 79, 4, WHITE );
    oled.drawPixel( 113,4, WHITE);
    oled.drawPixel( 113, 59, WHITE );
    oled.drawPixel( 79, 59, WHITE );
    oled.drawPixel( 47, 59, WHITE );
    oled.drawPixel( 15, 59, WHITE );
    oled.drawPixel( 15, 4, WHITE );
    oled.drawPixel( 47, 4, WHITE );

    oled.drawLine( 63, 32, 63 + 25 * cos( ( 270 + now.second() * 6 ) * 3.14159 / 180 ), 32 + 25 * sin( ( 270 + now.second() * 6 ) * 3.14159 / 180 ), WHITE );    

    oled.drawLine( 63, 32, 63 + 20 * cos( ( 270 + now.minute() * 6 ) * 3.14159 / 180 ), 32 + 20 * sin( ( 270 + now.minute() * 6 ) * 3.14159 / 180 ), WHITE );    

    oled.drawLine( 63, 32, 63 + 15 * cos( ( 270 + now.hour() % 12 * 30 + now.minute() * 0.5 ) * 3.14159 / 180 ),32 + 15 * sin( ( 270 + now.hour() % 12 * 30 + now.minute() * 0.5 ) * 3.14159 / 180 ), WHITE );    
  }

  else if(face == 6)//temperature sensor
  {
    if(A == 1) 
    {
      oled.drawBitmap(46, 14, temp, 36, 36,WHITE);
      oled.display();
      delay(1000);
      oled.clearDisplay();
      A++;
    }

    oled.setCursor(11,0);
    oled.print(F("Temperature:"));
    oled.print(getTemp());
    oled.print('F');
    for(int xaxis = 0; xaxis < 128; xaxis++)
    {
      if((int)temperature[xaxis] != -1) oled.drawPixel(xaxis, map((int)temperature[xaxis], graphMin, graphMax, 63, 8), WHITE);
    }
    oled.setCursor(0,9);
    oled.print(graphMax);
    oled.print('F');
    oled.setCursor(0,57);
    oled.print(graphMin);
    oled.print('F');
  }

  else if(face == 7)// stopwatch logic
  {
    if(B == 1)
    {
      oled.drawBitmap(46, 12, stopwatch, 36, 40,WHITE);
      oled.display();
      delay(1000);
      oled.clearDisplay();
      B++;
    }

    oled.setTextSize(2);
    oled.setTextColor(WHITE);
    oled.setCursor(2,25);

    // check for button press
    stopButtonState = digitalRead(cbtn);                   // read the button state and store

    if (stopButtonState == LOW && laststopButtonState == HIGH  &&  blinking == false)     // check for a high to low transition
      // if true then found a new button press while clock is not running - start the clock
    {
      startTime = millis();                                   // store the start time
      blinking = true;                                     // turn on blinking while timing
      delay(5);                                               // short delay to debounce switch
      laststopButtonState = stopButtonState;                          // store stopButtonState in laststopButtonState, to compare next time

    }

    else if (stopButtonState == LOW && laststopButtonState == HIGH && blinking == true)     // check for a high to low transition
      // if true then found a new button press while clock is running - stop the clock and report
    {
      elapsedTime =   millis() - startTime;              // store elapsed time
      blinking = false;                                  // turn off blinking, all done timing
      laststopButtonState = stopButtonState;                     // store stopButtonState in laststopButtonState, to compare next time
      displayStopwatch(elapsedTime);
    }

    else{
      laststopButtonState = stopButtonState;                         // store stopButtonState in laststopButtonState, to compare next time

        if(blinking == true)
      {
        runningTime = millis() - startTime; 
        displayStopwatch(runningTime);
      }

      if(blinking == false)
      {
        displayStopwatch(elapsedTime);
      }
    }
    sleepMillis = millis();
    digitalWrite(13,LOW);
  }

  else if (face == 8)// timeset
  {
    if(C == 1)
    {
      oled.drawBitmap(46, 14, settime, 36, 36,WHITE);
      oled.display();
      delay(1000);
      oled.clearDisplay();
      setMinute = now.minute();
      setHour = now.hour();
      setDay = now.day();
      setMonth = now.month();
      setYear = now.year()%100;
      C++;
    }

    oled.setCursor(13, 28);
    oled.print(F("Press to set time"));
    oled.setCursor(57, 57);
    oled.print('V');

    checkButtons();

    if(buttonState_c == LOW)// enter set mode when the center button is pressed
    {
      delay(100);
      set = true;
      delay(100);
    }

    while(set == true)
    {
      oled.clearDisplay();

      oled.setCursor(0, 55);
      oled.print(F("EXT"));
      oled.setCursor(110, 55);
      oled.print(F("SET"));

      oled.setCursor(22, 28);
      oled.print(setHour/10);
      oled.print(setHour%10);
      oled.print(':');
      oled.print(setHour/10);
      oled.print(setMinute%10); 
      oled.print(' '); 
      oled.print(setHour/10);
      oled.print(setMonth%10);
      oled.print('/'); 
      oled.print(setHour/10);
      oled.print(setDay%10); 
      oled.print('/');
      oled.print(setHour/10);
      oled.print(setYear%10);

      if(setSelect == 0) oled.drawLine(0, 63, 16, 63, WHITE);

      else if(setSelect == 6) oled.drawLine(110, 63, 127, 63, WHITE);

      if(setSelect > 0 && setSelect < 6)
      {
        oled.drawLine(((setSelect-1) * 18) + 22, 36, ((setSelect-1) * 18) + 33, 36, WHITE);
      }

      checkButtons();

      if(buttonState_r == LOW)// right advances the selection 
      {      
        delay(100);
        setSelect++; 
        if(setSelect > 6)
        {
          setSelect = 0;
        } 
      }

      if(buttonState_l == LOW)
      {
        delay(100);
        setSelect--;
        if(setSelect < 0)
        {
          setSelect = 6;
        }
      }

      if(buttonState_c == LOW)// when the center button is pressed, increment the value or set/exit
      {
        delay(100);

        switch (setSelect) {
        case 0:
          sleepMillis = millis();
          set = false;
          break;
        case 1:
          setHour++;
          if(setHour > 23) setHour = 0;
          break;
        case 2:
          setMinute++;
          if(setMinute > 59) setMinute = 0;
          break;
        case 3:
          setMonth++;
          if(setMonth > 12) setMonth = 1;
          break;
        case 4:
          setDay++;
          if(setDay > 31) setDay = 1;
          break;
        case 5:
          setYear++;
          break;
        case 6:
          DateTime now = RTC.now();
          DateTime updated = DateTime(2000 + setYear, setMonth, setDay, setHour, setMinute, 0);
          RTC.adjust(updated);
          RTC.begin();
          oled.setCursor(49, 28);
          oled.clearDisplay();
          oled.print(F("Done!"));
          oled.display();
          delay(1000);
          sleepMillis = millis();
          set = false;
          break;
        }
      }

      oled.display();
      oled.clearDisplay();
    } 
  }

  else if(face == 9)
  {

    if(D == 1)
    {
      oled.drawBitmap(46, 14, brightnessIcon[0], 36, 36,WHITE);
      oled.display();
      delay(1000);
      oled.clearDisplay();
      D++;
    }

    checkButtons();

    if(buttonState_c == LOW)
    {
      delay(100);
      brightnessLevel++; 
      if(brightnessLevel > 4)
      {
        brightnessLevel = 1;
      } 
    }

    oled.setCursor(27,56);
    oled.setTextSize(1);
    oled.print(F("Brightness: "));
    oled.print(brightnessLevel);
    oled.drawBitmap(46, 14, brightnessIcon[brightnessLevel] ,36 , 36 ,WHITE);
    if(brightnessLevel == 1) oled.setBrightness(1); 
    else oled.setBrightness((brightnessLevel*50)); 
  }

  if (face != 6)// these reset variables used to display the icons when the face changes
  {
    A = 1;
  }

  if (face != 7)
  {
    B = 1;
    if ( (millis() - previousMillis > 500) ) {

      if (blinking == true)
      {
        previousMillis = millis();// remember the last time we blinked the LED

        // if the LED is off turn it on and vice-versa.
        if (value == LOW)
          value = HIGH;
        else
          value = LOW;
        digitalWrite(13, value);
      }
    }
  }

  if (face != 8)
  {
    C = 1;
  }

  if (face != 9)
  {
    D = 1;
  }

  else digitalWrite(13, LOW);

  if(millis() - sleepMillis >= wakeTime)// after a period of no activity, send all devices into sleep mode
  {
    oled.clearDisplay();
    oled.display();

    Wire.beginTransmission(0x18);//// MCP9808 temperature sensor shutdown v
    Wire.write((uint8_t)0x01);
    Wire.endTransmission();
    Wire.requestFrom(0x18,2);
    byte bytebuffer1 = Wire.read(); //upper bits first 15-8
    byte bytebuffer2 = Wire.read(); //then bits 7-0//set “bit 8″ to “1” to enter shut down mode
    bytebuffer1 |= (1 << 0);
    //x |= (1 << n); // forces nth bit of x to be 1. all other bits left alone
    Wire.beginTransmission(0x18);
    Wire.write((uint8_t)0x01); //register pointer
    Wire.write(bytebuffer1); //
    Wire.write(bytebuffer2); //this one is unchanged probably could skip it
    Wire.endTransmission();

    sleepNow();// sleep function called here, loop resumes from here when it wakes up

    Wire.beginTransmission(0x18);// wake the MCP9808 temperature sensor v
    Wire.write((uint8_t)0x01);
    Wire.endTransmission();
    Wire.requestFrom(0x18,2);
    bytebuffer1 = Wire.read(); //upper MSB bits 16-8
    bytebuffer2 = Wire.read(); //then LSB bits 7-0//set bit 8 to 0 = Continuous conversion (power-up default)
    // x &= ~(1 << n); // forces nth bit of x to be 0. all other bits left alone.
    bytebuffer1 &= ~(1 << 0);
    Wire.beginTransmission(0x18);
    Wire.write((uint8_t)0x01); //register pointer
    Wire.write(bytebuffer1);
    Wire.write(bytebuffer2); //this one is unchanged probably could skip it
    Wire.endTransmission();

    oled.fullOn();
  }

  ////////////////////////////////////////////////////////
  if(readVcc() > 4200)
  {
    if (lipostat < 512)// if the battery charge status is low, it's charging
    {
      oled.clearDisplay();
      oled.setBrightness(255);
      oled.drawBitmap(48, 16, battplug, 32, 32, WHITE);
      oled.setCursor(48,57);
      oled.print(readVcc());
      oled.print(F("mV"));
      analogWrite(charge, brightness);    
      // change the brightness for next time through the loop:
      brightness = brightness + fadeAmount;
      // reverse the direction of the fading at the ends of the fade: 
      if (brightness == 0 || brightness == 100) {
        fadeAmount = -fadeAmount ; 
      }     
      digitalWrite(full, LOW);
    }

    else if(lipostat > 512)// if the battery charging state is high, and it's been plugged in and charging before this, show full
    {
      oled.clearDisplay();
      oled.setBrightness(255);
      oled.drawBitmap(48, 16, fullbatt , 32, 32, WHITE);
      digitalWrite(full, HIGH);
      digitalWrite(charge, LOW); 
    }
    sleepMillis = millis();
  }

  else{
    digitalWrite(charge, LOW); 
    digitalWrite(full, LOW); 
  }

  while (readVcc() <= 3100)// if the power gets low, the battery dead animation shows
  {
    oled.setBrightness(255);
    oled.clearDisplay();
    oled.drawBitmap(48, 16, emptybatt, 32, 32, WHITE);
    delay(250);
    sleepMillis = millis();
  }

  ///////////////////////////////////

  oled.display();
  oled.clearDisplay();
}

// font displaying methods and other program methods

void textNum(int x, int y, int n)
{
  if(n < 13)
  {
    oled.drawBitmap(x, y, text[n-1], 72, 16, WHITE);
  }
  else if (n > 12 && n < 20)
  {
    oled.drawBitmap(x, y, text2[n-13], 128, 16, WHITE);
  }
  else if(n > 19)
  {
    oled.drawBitmap(x, y, text3[n/10-2], 72, 16, WHITE);
  }
}

void displayNum(int x, int y, int n)
{
  oled.drawBitmap(x, y, font[n], 25, 28, WHITE);
}

void blindNum(int x, int y, int n)
{
  oled.drawBitmap(x, y, blinds[n], 22, 64, WHITE);
}

void displayBoxFont(int x, int y, int n)
{
  oled.drawBitmap(x, y, box[n], 30, 64, WHITE);
}

void displayStopwatch(long input)
{
  hours = (int)(input / 3600000L);
  minutes = (int)(input / 60000L) % 60;
  seconds = (int)(input / 1000L) % 60;

  if(hours < 10)
  {
    oled.print(F("0")); 
  }

  oled.print(hours); 

  oled.print(F(":"));                             // print colon

  if( minutes < 10)
  {
    oled.print(F("0")); 
  }

  oled.print(minutes); 

  oled.print(F(":"));                             // print colon

  if(seconds < 10)
  {
    oled.print(F("0")); 
  }

  oled.print(seconds);         

  oled.print(F(":"));                             // print colon

  // use modulo operator to get fractional part of time 
  fractional = (int)(input % 1000L);
  oled.setTextSize(1);
  // pad in leading zeros - wouldn't it be nice if 
  // Arduino language had a flag for this? :)
  if (fractional == 0)
    oled.print(F("00"));      // add three zero's
  else if (fractional < 10)    // if fractional < 10 the 0 is ignored giving a wrong time, so add the zeros
    oled.print(F("00"));       // add two zeros
  else if (fractional < 100)
    oled.print(F("0"));        // add one zero
  oled.print(fractional);  // print fractional part of time 
}

void checkButtons()
{
  buttonState_l = digitalRead(lbtn);
  buttonState_c = digitalRead(cbtn);
  buttonState_r = digitalRead(rbtn);
}

int readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  return (int)result;
}

void checkBatteryLevel()
{
  percent = map(readVcc(), 3000, 3780, 0, 100);
  if(percent > 100) percent = 100;
}

double getTemp()
{
  f = tempsensor.readTempC() * 1.8 + 32;
  return f-7.0;//wearing the watch and heating due to VCC add ~7F to the reading
}

void wakeUpNow()// here the interrupt is handled after wakeup
{
  // execute code here after wake-up before returning to the loop() function
  // timers and code using timers (serial.print and more...) will not work here.
}

void sleepNow()// here we put the arduino to sleep
{
  /* Now is the time to set the sleep mode. In the Atmega8 datasheet
   * http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
   * there is a list of sleep modes which explains which clocks and 
   * wake up sources are available in which sleep mode.
   *
   * In the avr/sleep.h file, the call names of these sleep modes are to be found:
   *
   * The 5 different modes are:
   *     SLEEP_MODE_IDLE         -the least power savings 
   *     SLEEP_MODE_ADC
   *     SLEEP_MODE_PWR_SAVE
   *     SLEEP_MODE_STANDBY
   *     SLEEP_MODE_PWR_DOWN     -the most power savings
   *
   * For now, we want as much power savings as possible, so we 
   * choose the according 
   * sleep mode: SLEEP_MODE_PWR_DOWN
   * 
   */
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here

  sleep_enable();          // enables the sleep bit in the mcucr register
  // so sleep is possible. just a safety pin 

  /* Now it is time to enable an interrupt. We do it here so an 
   * accidentally pushed interrupt button doesn't interrupt 
   * our running program. if you want to be able to run 
   * interrupt code besides the sleep function, place it in 
   * setup() for example.
   * 
   * In the function call attachInterrupt(A, B, C)
   * A   can be either 0 or 1 for interrupts on pin 2 or 3.   
   * 
   * B   Name of a function you want to execute at interrupt for A.
   *
   * C   Trigger mode of the interrupt pin. can be:
   *             LOW        a low level triggers
   *             CHANGE     a change in level triggers
   *             RISING     a rising edge of a level triggers
   *             FALLING    a falling edge of a level triggers
   *
   * In all but the IDLE sleep modes only LOW can be used.
   */

  attachInterrupt(0,wakeUpNow, LOW); // use interrupt 0 (pin 2) and run function
  // wakeUpNow when pin 2 gets LOW 

    sleep_mode();            // here the device is actually put to sleep!!
  // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP

  sleep_disable();         // first thing after waking from sleep:
  // disable sleep...
  detachInterrupt(0);      // disables interrupt 0 on pin 2 so the 
  // wakeUpNow code will not be executed 
  // during normal running time.
}
