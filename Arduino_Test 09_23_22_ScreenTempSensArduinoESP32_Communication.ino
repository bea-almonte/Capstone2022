#include <OneWire.h>
#include <DallasTemperature.h>
#include <UTFT.h>
#define RXp2 17
#define TXp2 16
#include "DS3231.h"                   // Download this library from the link given below

//****Sensor Setup Section****//
#define ONE_WIRE_BUS 4                // Data wire is conntec to the Arduino digital pin 4
OneWire oneWire(ONE_WIRE_BUS);        // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);  // Pass our oneWire reference to Dallas Temperature sensor 
char sensorPrintoutC[6];              // creates CHAR array for Celcius to print to screen
char sensorPrintoutF[6];              // creates CHAR array for Fahrenheit to print to screen

// ***TFT Screen Setup***//
UTFT myGLCD(ILI9486,38,39,40,41);     //(Board type, pin,pin,pin,pin) used for UTFT LCD Shield
extern uint8_t SmallFont[];           // Declare which fonts we will be using 16x16
extern uint8_t BigFont[];             // Declare which fonts we will be using 24x36
extern uint8_t Ubuntu[];              // Declare which fonts we will be using
extern uint8_t SevenSegNumFont[];     // Declare which fonts we will be using

//****RTC Pin Set****//
DS3231  rtc(20, 21);                  //Define SDA and SCL pin here

void setup()
{
  randomSeed(analogRead(0));          //Don't know what this is, I think its UTFT screen stuff
  //RTC Communication                 
  Serial.begin(115200);               // Set baud rate for communication RTC
  rtc.begin();                        // Initiate RTC module
  Serial.begin(9600);                 // Starts Serial Comm 1
  sensors.begin();                    //Start the Sensors  
// Setup the LCD
  myGLCD.InitLCD();
  myGLCD.clrScr();
}

void loop()
{
  int buf[478];
  int x, x2;
  int y, y2;
  int r;
  
//Get temperatures C
 sensors.requestTemperatures(); 
  float temperatureC0 = sensors.getTempCByIndex(0);
  float temperatureC1 = sensors.getTempCByIndex(1);
  float averageTempC = ((temperatureC0 + temperatureC1) /2);
  String sensorValC = String(averageTempC);
  sensorValC.toCharArray(sensorPrintoutC, 6);

//Get temperatures F
 sensors.requestTemperatures(); 
  float temperatureF0 = sensors.getTempFByIndex(0);
  float temperatureF1 = sensors.getTempFByIndex(1);
  float averageTempF = ((temperatureF0 + temperatureF1) /2);
  String sensorValF = String(averageTempF);
  sensorValF.toCharArray(sensorPrintoutF, 6);

// ***THIS SECTION WILL BE FOR HEADINGS
myGLCD.setBackColor(VGA_BLACK);
myGLCD.setColor(VGA_WHITE); 

//***SET TEMP Section*****//
  myGLCD.setFont(Ubuntu);
  myGLCD.setColor(VGA_WHITE); 
  myGLCD.print("F- Temp:", LEFT, 100);
  myGLCD.print(sensorPrintoutF, 313, 100);//THIS WILL CHANGE WITH USER SET VALUE****  
  
  myGLCD.setFont(BigFont);
  myGLCD.print("o",433,94); 

  myGLCD.setFont(Ubuntu);
  myGLCD.print("F",449,100); 
  
  //***Current TEMP Section*****//
  myGLCD.setFont(Ubuntu);
  myGLCD.setColor(VGA_WHITE); 
  myGLCD.print("Current Temp:", LEFT, 146);
  myGLCD.print(sensorPrintoutC, 313, 146);

  myGLCD.setFont(BigFont);
  myGLCD.print("o",433,140); 

  myGLCD.setFont(Ubuntu);
  myGLCD.print("C",449,146); 
  
//DATE 
  myGLCD.setBackColor(VGA_BLACK);
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255,255,255);
  myGLCD.print(rtc.getDOWStr(), RIGHT, 0);
  myGLCD.print(rtc.getDateStr(FORMAT_MIDDLEENDIAN,FORMAT_SHORT,'/'), RIGHT, 24);
//TIME   
  myGLCD.setColor(255,255,255);
  myGLCD.print(rtc.getTimeStr(), RIGHT, 48);
//TITLE
  myGLCD.setColor(VGA_RED);
  myGLCD.fillRect(0, 75, 483, 80);

  myGLCD.setFont(Ubuntu);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.print("Thermo", LEFT , 0);
  myGLCD.setColor(VGA_RED);
  myGLCD.print("STORE", LEFT , 36);
  myGLCD.setColor(VGA_RED);
  myGLCD.fillRect(0, 250, 483, 255);
  delay(2000);
  
//   for (int i=1; i<10; i++)
//   {
//     myGLCD.setColor(VGA_RED);
//     myGLCD.fillRect(0, 256, 483, 320);
//     delay(1000);
//     myGLCD.setColor(VGA_BLACK);
//     myGLCD.fillRect(0, 256, 483, 320);
//     delay(1000);
//   }
//     for (int i=1; i<10; i++)
//   {
//     myGLCD.setColor(VGA_GREEN);
//     myGLCD.fillRect(0, 256, 483, 320);
//     delay(1000);
//   }
//   
//   delay(2000);
}
