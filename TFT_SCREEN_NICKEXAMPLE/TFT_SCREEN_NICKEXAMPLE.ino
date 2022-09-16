// UTFT_Demo_480x320 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// This program is a demo of how to use most of the functions
// of the library with a supported display modules.
//
// This demo was made for modules with a screen resolution 
// of 480x320 pixels.
//
// This program requires the UTFT library.
//

#include <UTFT.h>
//#include <avr/pgmspace.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t Ubuntu[];
extern uint8_t SevenSegNumFont[];
//extern unsigned int left[0x400];
//extern unsigned int right[0x400];
//extern unsigned int up[0x400];
//extern unsigned int down[0x400];
//extern unsigned int settings[0x400];



UTFT myGLCD(ILI9486,38,39,40,41);


void setup()
{
  randomSeed(analogRead(0));
  
// Setup the LCD
  myGLCD.InitLCD();
  //myGLCD.setFont(SmallFont);
  //myGLCD.setFont(BigFont);
  myGLCD.clrScr();
  //myGLCD.setBackColor(VGA_BLACK);
}

void loop()
{
  int buf[478];
  int x, x2;
  int y, y2;
  int r;

// Clear the screen
  //myGLCD.clrScr();
  
// ***THIS SECTION WILL BE FOR HEADINGS***//

//TEMP Section
  myGLCD.setColor(VGA_WHITE); 
  myGLCD.print("Temperature:", CENTER, 90);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.print("72", CENTER, 135); 
  myGLCD.setFont(BigFont);
  myGLCD.print("o",275,130); 
  
//DATE 
  myGLCD.setBackColor(VGA_BLACK);
  myGLCD.setFont(SmallFont);
  myGLCD.setColor(255,255,255);
  myGLCD.print("DATE", 350, 0);
//TIME   
  myGLCD.setColor(255,255,255);
  myGLCD.print("TIME", 400, 0);
//TITLE
  myGLCD.setColor(VGA_RED);
  //myGLCD.setColor(64, 64, 64);
  myGLCD.fillRect(0, 65, 483, 67);
  //myGLCD.drawLine(0,52,470,52);
  myGLCD.setFont(Ubuntu);
  myGLCD.setColor(VGA_WHITE);
  
  myGLCD.print("Thermo", LEFT , 0);
  myGLCD.setColor(VGA_RED);
  myGLCD.print("STORE", 120 , 28);
  
  myGLCD.setColor(VGA_RED);
  myGLCD.fillRect(0, 250, 483, 255);
  delay(2000);
  
  for (int i=1; i<10; i++)
  {
    myGLCD.setColor(VGA_RED);
    myGLCD.fillRect(0, 256, 483, 320);
    delay(1000);
    myGLCD.setColor(VGA_BLACK);
    myGLCD.fillRect(0, 256, 483, 320);
    delay(1000);
  }
    for (int i=1; i<10; i++)
  {
    myGLCD.setColor(VGA_GREEN);
    myGLCD.fillRect(0, 256, 483, 320);
    delay(1000);
  }
  delay(2000);


  

// Buttons (if needed)
//  //myGLCD.setBackColor(VGA_BLACK);
//  myGLCD.drawBitmap (32, 280, 32, 32, left);
//  myGLCD.fillRect(85, 250, 90, 320);
//  
//  myGLCD.drawBitmap (119, 280, 32, 32, up);
//  myGLCD.fillRect(172, 250, 177, 320);
//  
//  myGLCD.drawBitmap (206, 280, 32, 32, right);
//  myGLCD.fillRect(259, 250, 264, 320);
//  
//  myGLCD.drawBitmap (293, 280, 32, 32, down);
//  myGLCD.fillRect(346, 250, 351, 320);
//  
//  myGLCD.drawBitmap (380, 280, 32, 32, settings);

  //delay(2);
}
