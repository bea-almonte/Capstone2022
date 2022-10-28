#include <OneWire.h>
#include <DallasTemperature.h>
#include <UTFT.h>
#include "DS3231.h"                   // Download this library from the link given below

//UART COMMUNICATION PIN SET
#define RXp2 17
#define TXp2 16

//KEYPAD Setup
//ground on keyoard is pin with 1 on it...
#define next 9//connect next key to pin 9 (opposite end of ground)
#define bef 10//connect before key to pin 10
#define inc 11 //connect increment key to pin 11
#define dec 12//connect decrement key to pin 12
#define sel 13 //conect select key to pin 13 (closest to ground)

//****Sensor Setup Section****//
#define ONE_WIRE_BUS 4                // Data wire is conntec to the Arduino digital pin 4
OneWire oneWire(ONE_WIRE_BUS);        // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);  // Pass our oneWire reference to Dallas Temperature sensor 
char sensorPrintoutC[6];              // creates CHAR array for Celcius to print to screen
char sensorPrintoutF[6];              // creates CHAR array for Fahrenheit to print to screen
//char sensorPrintoutHS[6];              // creates CHAR array for Heat Sink to print to screen
//char sensorPrintoutHSF[6];              // creates CHAR array for Heat Sink to print to screen

//*****Test sensor printouts*****//
char testC0[6];                        // creates CHAR array for Sensor 0 to print to screen
char testC1[6];                        // creates CHAR array for Sensor 1 to print to screen
//char testF2[6];                        // creates CHAR array for Sensor 2 to print to screen


// ***TFT Screen Setup***//
UTFT myGLCD(ILI9486,38,39,40,41);     //(Board type, pin,pin,pin,pin) used for UTFT LCD Shield
extern uint8_t SmallFont[];           // Declare which fonts we will be using 16x16
extern uint8_t BigFont[];             // Declare which fonts we will be using 24x36
extern uint8_t Ubuntu[];              // Declare which fonts we will be using
extern uint8_t SevenSegNumFont[];     // Declare which fonts we will be using

//****RTC Pin Set****//
DS3231  rtc(20, 21);                  //Define SDA and SCL pin here

int Relay1 = 2;                       //
int Relay2 = 3;                       //
int Relay3 = 5;                       //
int Relay4 = 6;                       //
int Relay5 = 7;                       //
int Relay6 = 8;                       //
int selPush = 0;
float setTemp = 37.4;                //user defined variable default is 3 degrees celcius
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
  myGLCD.setColor(VGA_WHITE);
  myGLCD.fillRect(0, 256, 483, 320);

//RELAY MODULE SETUP
  pinMode(Relay1,OUTPUT);
  pinMode(Relay2,OUTPUT);
  pinMode(Relay3,OUTPUT);
  pinMode(Relay4,OUTPUT);
  pinMode(Relay5,OUTPUT);
  pinMode(Relay6,OUTPUT);
  //Setup the Relays;
    digitalWrite(Relay1, HIGH);
    digitalWrite(Relay2, HIGH);
    digitalWrite(Relay3, HIGH);
    digitalWrite(Relay4, HIGH);
    digitalWrite(Relay5, HIGH);
    digitalWrite(Relay6, HIGH);
//KEYPAD PINMODE  
  pinMode(sel, INPUT_PULLUP); //set pin as input
  pinMode(inc, INPUT_PULLUP); //set pin as input
  pinMode(dec, INPUT_PULLUP); //set pin as input
  pinMode(next, INPUT_PULLUP); //set pin as input
  pinMode(bef, INPUT_PULLUP); //set pin as input

}

void loop()
{
  int buf[478];
  int x, x2;
  int y, y2;
  int r;
  int selstatus = digitalRead(sel); //read if sel is pressed
  int incstatus = digitalRead(inc); //read if inc is pressed
  int decstatus = digitalRead(dec); //read if dec is pressed
  int nextstatus = digitalRead(next); //read if next is pressed
  int befstatus = digitalRead(bef); //read if bef is pressed
  
  
//Get temperatures C
  sensors.requestTemperatures(); 
  float temperatureC0 = sensors.getTempCByIndex(0);
  float temperatureC1 = sensors.getTempCByIndex(1);
  float averageTempC = ((temperatureC0 + temperatureC1) /2);
  String sensorValC = String(averageTempC);
  sensorValC.toCharArray(sensorPrintoutC, 6);

//Get temperatures F
  //sensors.requestTemperatures(); 
  //float temperatureF0 = sensors.getTempFByIndex(0);
  //float temperatureF1 = sensors.getTempFByIndex(1);
 // float averageTempF = ((temperatureF0 + temperatureF1) /2);
  String sensorValF = String(setTemp);
  sensorValF.toCharArray(sensorPrintoutF, 6);

//Get temperatures HS area
 sensors.requestTemperatures(); 
  float temperatureC2 = sensors.getTempCByIndex(2);
  float temperatureC2F = sensors.getTempFByIndex(2);
  
  String sensorValHS= String(temperatureC2);
  String sensorValHSF= String(temperatureC2F);
  //sensorValHS.toCharArray(sensorPrintoutHS, 6);
  //sensorValHSF.toCharArray(sensorPrintoutHSF, 6);


// ***THIS SECTION WILL BE FOR HEADINGS
  myGLCD.setBackColor(VGA_BLACK);
  myGLCD.setColor(VGA_WHITE); 

//***SET TEMP Section*****//
  myGLCD.setFont(Ubuntu);
  myGLCD.setColor(VGA_WHITE); 
  myGLCD.print("Set Temp:", LEFT, 100);
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

//***Display sensor values for testing***//
  float test0 = sensors.getTempCByIndex(0);
  float test1 = sensors.getTempCByIndex(1);
  //float test2 = sensors.getTempCByIndex(2); 
  String sensorVal0= String(test0);
  String sensorVal1= String(test1);
  //String sensorVal2= String(test2);
  sensorVal0.toCharArray(testC0, 6);
  sensorVal1.toCharArray(testC1, 6);
 // sensorVal2.toCharArray(testF2, 6);
     
  myGLCD.setFont(BigFont);
  myGLCD.setColor(VGA_WHITE); 
  myGLCD.print("Sensor Low(0):", LEFT, 190);
  myGLCD.print(testC0, 245, 190);

       
  myGLCD.setFont(BigFont);
  myGLCD.setColor(VGA_WHITE); 
  myGLCD.print("Sensor High(1):", LEFT, 210);
  myGLCD.print(testC1, 245, 210);

  // myGLCD.setFont(BigFont);
  // myGLCD.setColor(VGA_RED); 
  // myGLCD.print("Sensor HS(2):", LEFT, 230);
  // myGLCD.print(testF2, 245, 230);

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

  // Serial.println();
  // Serial.print("Lower Sensor(0): ");
  // Serial.print(sensors.getTempCByIndex(0));
  // Serial.println(" C");

  // Serial.print("Upper Sensor(1): ");
  // Serial.print(sensors.getTempCByIndex(1));
  // Serial.println(" C");
  // Serial.print("HS Area: ");
  // Serial.print(sensors.getTempFByIndex(2));
  // Serial.println(" F");
  // Serial.println();

  if(averageTempC <= 4 && averageTempC > 0)
  {

    myGLCD.setColor(VGA_GREEN);
   // myGLCD.fillRect(0, 256, 483, 320);
    myGLCD.setBackColor(VGA_WHITE);
   // myGLCD.setColor(VGA_BLACK);
    myGLCD.print("Good Operating Range", CENTER, 270);
  }
  else
  {
    myGLCD.setColor(VGA_WHITE);
    myGLCD.fillRect(0, 256, 483, 320);
    myGLCD.setBackColor(VGA_WHITE);
    myGLCD.setColor(VGA_RED);
    myGLCD.print("OUT OF RANGE", CENTER, 270);
  }
  if(averageTempC < 3.50 && averageTempC > 2){
    if(temperatureC0 < 2.5 ){
      digitalWrite(Relay1,LOW);     // THIS TURNS OFF THE POWER TO THE COOL SETTING
      }
      else{
        digitalWrite(Relay1,HIGH);
      }
    if(temperatureC1 < 3 ){
      digitalWrite(Relay4,LOW);     // THIS TURNS OFF THE POWER TO THE COOL SETTING for 2/4
      }
      else{
        digitalWrite(Relay4,HIGH);
      }
    }
    else
  {
    digitalWrite(Relay1,HIGH);     // THIS TURNS ON THE POWER TO THE COOL SETTING
    digitalWrite(Relay4,HIGH);     // THIS TURNS ON THE POWER TO THE COOL SETTING for 2/4
  }
  if(!selstatus){                 //HOLD for 3 Seconds to increase selPush!!!!
    //delay(2000);
    selPush=1;
    Serial.println(selPush);
  }
  while(selPush < 3 && selPush>0){      //NEED TO CHANGE THIS SECTION!!!!!!
       myGLCD.setColor(VGA_WHITE);
       myGLCD.drawRect(310,90,475,135);
       delay(3000);
       if(!incstatus){
         setTemp++;
       }
       if(!selstatus){
         //delay(2000);
         selPush++;
         Serial.println(selstatus);
       }
       
    }
  if(selPush>3){
      selPush = 0;
       myGLCD.setColor(VGA_BLACK);
       myGLCD.drawRect(310,90,475,135); 
    }
  Serial.println(selstatus);
}
