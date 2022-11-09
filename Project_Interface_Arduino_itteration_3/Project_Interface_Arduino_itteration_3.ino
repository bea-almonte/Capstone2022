#include <OneWire.h>
#include <DallasTemperature.h>
#include <UTFT.h>
#include "DS3231.h"                   // Download this library from the link given below

//UART COMMUNICATION PIN SET
#define RXp2 17
#define TXp2 16

void SendTemp(float currentTemp);

//KEYPAD Setup
//ground on keyoard is pin with 1 on it...
#define sel 18 //conect select key to pin 13
#define inc 2 //connect increment key to pin 11
#define dec 3//connect decrement key to pin 12

bool selectOn = false;
//interrupt variables
volatile unsigned long int trigger = 0; // time when triggered
volatile unsigned long int pressed = 0; // time since triggered

void SendTemp(String timestampString);
void SendInterval(int timeInMinutes);
void SendWifiChoice(int wifiChoice);
//void fanSwitch(int,int,int,int,int);
void selISR();
void incISR();
void decISR();

//****Sensor Setup Section****//
#define ONE_WIRE_BUS_ONE 4                // Data wire is conntec to the Arduino digital pin 4
#define ONE_WIRE_BUS_TWO 49
OneWire oneWire(ONE_WIRE_BUS_ONE);        // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);  // Pass our oneWire reference to Dallas Temperature sensor


//Get temperatures C
float temperatureC0;
float temperatureC1;
float averageTempC;

const int fanOnDuration = 30000;
const long fanOffDuration = 120000;

unsigned long currentMillis = 0; //
unsigned long previousMillis = 0; //


String sensorValString;
String setTempString;

// ***TFT Screen Setup***//
UTFT myGLCD(ILI9486, 38, 39, 40, 41); //(Board type, pin,pin,pin,pin) used for UTFT LCD Shield
extern uint8_t SmallFont[];           // Declare which fonts we will be using 16x16
extern uint8_t BigFont[];             // Declare which fonts we will be using 24x36
extern uint8_t Ubuntu[];              // Declare which fonts we will be using
extern uint8_t SevenSegNumFont[];     // Declare which fonts we will be using

//****FUNCTIONS****
void printHeader();
void CheckRange(float averageTempC);
void DisplayCurrentTemps(int setTemp, float sensorVal);
bool InRange(float averageTempC);
//****RTC Pin Set****//
DS3231  rtc(20, 21);                  //Define SDA and SCL pin here

int Relay1 = 5;                       //
int Relay2 = 6;                       //
int Relay3 = 7;                       //
int Relay4 = 8;                       //
int Relay5 = 9;                       //
int Relay6 = 10;                      //
int Relay7 = 47;                      //
float setTemp = 5;                    //user defined variable default is 25 degrees celcius
void setup()
{

  // KEYPAD SETUP
  pinMode(sel, INPUT_PULLUP); //set pin as input
  pinMode(inc, INPUT_PULLUP); //set pin as input
  pinMode(dec, INPUT_PULLUP); //set pin as input
  attachInterrupt(digitalPinToInterrupt(sel), selISR, HIGH); // set
  attachInterrupt(digitalPinToInterrupt(inc), incISR, HIGH); // inc
  attachInterrupt(digitalPinToInterrupt(dec), decISR, HIGH); // dec

  // START EVERYTHING ELSE
  randomSeed(analogRead(0));          //Don't know what this is, I think its UTFT screen stuff
  //RTC Communication
  Serial.begin(115200);               // Set baud rate for communication RTC
  rtc.begin();                        // Initiate RTC module
  Serial.begin(9600);                 // Starts Serial Comm 1
  Serial2.begin(9600); // CONNECT W ESP
  sensors.begin();                    //Start the Sensors
  // Setup the LCD
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setColor(VGA_WHITE);
  myGLCD.fillRect(0, 256, 483, 320);

  //RELAY MODULE SETUP
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  pinMode(Relay5, OUTPUT);
  pinMode(Relay6, OUTPUT);
  pinMode(Relay7, OUTPUT);
  //Setup the Relays;
  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);
  digitalWrite(Relay3, HIGH);
  digitalWrite(Relay4, HIGH);
  digitalWrite(Relay5, HIGH);
  digitalWrite(Relay6, HIGH);
  digitalWrite(Relay7, LOW);

}

void loop()
{
  currentMillis = millis();
  PrintHeader();
  int buf[478];
  int x, x2;
  int y, y2;
  int r;

  //Get temperatures C
  sensors.requestTemperatures();
  temperatureC0 = sensors.getTempCByIndex(0);
  temperatureC1 = sensors.getTempCByIndex(1);
  averageTempC = ((temperatureC0 + temperatureC1) / 2);

  // ***THIS SECTION WILL BE FOR HEADINGS
  myGLCD.setBackColor(VGA_BLACK);
  myGLCD.setColor(VGA_WHITE);

  if (selectOn) {
    PrintChangeTemp();
  } else {
    DisplayCurrentTemps(setTemp, averageTempC);
    DisplayBothTemps();
  }
  SendTemp(averageTempC);
  CheckRange(averageTempC);

  // RELAYS
  // ALL OFF WHEN -2 DEGREES BELOW
  // HALF OFF WHEN -0.5 DEGREES BELOW
  // ON WHEN ABOVE
  if(digitalRead(Relay7) == LOW){
    // Serial.print("Relay HIGH is: ");
    // Serial.println(digitalRead(Relay7));
    // Serial.print("Current Millis is: ");
    // Serial.println(currentMillis);
    // Serial.print("Previous Millis is: ");
    // Serial.println(previousMillis);
    // Serial.println(" ");
    if(currentMillis - previousMillis >= fanOnDuration){
      digitalWrite(Relay7,HIGH);
      // Serial.println();
      // Serial.println("RELAY JUST WENT LOW");
      // Serial.println();
      previousMillis = currentMillis;
    }
  }
  if(digitalRead(Relay7) == HIGH){
    // Serial.print("Relay LOW is: ");
    // Serial.println(digitalRead(Relay7));
    // Serial.print("Current Millis is: ");
    // Serial.println(currentMillis);
    // Serial.print("Previous Millis is: ");
    // Serial.println(previousMillis);
    // Serial.println(" ");
     if((currentMillis - previousMillis) >= fanOffDuration){
      digitalWrite(Relay7, LOW);
      //       Serial.println();
      // Serial.println("RELAY JUST WENT high");
      // Serial.println();
      previousMillis = currentMillis;
    }
  }  
   
   
  if ((averageTempC < (setTemp - 0.5)) && (averageTempC > (setTemp - 2))) {
    if (averageTempC < (setTemp - 1.5) ) {
      digitalWrite(Relay1, LOW);    // THIS TURNS OFF THE POWER TO THE COOL SETTING
      digitalWrite(Relay7, LOW);    // THIS TURNS OFF THE LID FAN
    }
    else {
      digitalWrite(Relay1, HIGH);
    }
    if (averageTempC < (setTemp - 1) ) {
      digitalWrite(Relay4, LOW);    // THIS TURNS OFF THE POWER TO THE COOL SETTING for 2/4
    }
    else {
      digitalWrite(Relay4, HIGH);
    }
  }
  else
  {
    digitalWrite(Relay1, HIGH);    // THIS TURNS ON THE POWER TO THE COOL SETTING
    digitalWrite(Relay4, HIGH);    // THIS TURNS ON THE POWER TO THE COOL SETTING for 2/4
  }
  //delay(5000);                   // ***I DON'T THINK WE NEED A DELAY AT ALL
}

void PrintChangeTemp() {


  //PrintHeader();
  if (setTemp < 10) {
    myGLCD.setColor(VGA_BLACK);
    myGLCD.fillRect(0, 80, 500, 230);
  }
  myGLCD.setFont(Ubuntu);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.print("Set Temp:", CENTER, 100);
  myGLCD.printNumI(setTemp, CENTER, 150);
}

void DisplayCurrentTemps(int setTemp, float sensorVal) {
  //***SET TEMP Section*****//
  myGLCD.setFont(Ubuntu);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.print("Set Temp:", LEFT, 100);
  myGLCD.printNumF(setTemp, 2, 313, 100); //THIS WILL CHANGE WITH USER SET VALUE****

  myGLCD.setFont(BigFont);
  myGLCD.print("o", 433, 94);

  myGLCD.setFont(Ubuntu);
  myGLCD.print("C", 449, 100);

  //***Current TEMP Section*****//
  myGLCD.setFont(Ubuntu);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.print("Current Temp:", LEFT, 146);
  myGLCD.printNumF(sensorVal, 2, 313, 146);

  myGLCD.setFont(BigFont);
  myGLCD.print("o", 433, 140);

  myGLCD.setFont(Ubuntu);
  myGLCD.print("C", 449, 146);
}
void PrintHeader() {
  //DATE
  myGLCD.setBackColor(VGA_BLACK);
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print(rtc.getDOWStr(), RIGHT, 0);
  myGLCD.print(rtc.getDateStr(FORMAT_MIDDLEENDIAN, FORMAT_SHORT, '/'), RIGHT, 24);
  //TIME
  myGLCD.setColor(255, 255, 255);
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
}

void CheckRange(float averageTempC) {
  myGLCD.setFont(Ubuntu);
  if (InRange(averageTempC)) {
    myGLCD.setColor(VGA_GREEN);
    myGLCD.setBackColor(VGA_WHITE);
    myGLCD.print("Good Operating Range", CENTER, 270);
  } else {
    myGLCD.setColor(VGA_WHITE);
    myGLCD.fillRect(0, 256, 483, 320);
    myGLCD.setBackColor(VGA_WHITE);
    myGLCD.setColor(VGA_RED);
    myGLCD.print("OUT OF RANGE", CENTER, 270);
  }
}

void DisplayBothTemps() {
  //***Display sensor values for testing***//
  float test0 = sensors.getTempCByIndex(0);
  float test1 = sensors.getTempCByIndex(1);

  myGLCD.setFont(BigFont);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.print("Sensor Low(0):", LEFT, 190);
  myGLCD.printNumF(test0, 2, 245, 190);


  myGLCD.setFont(BigFont);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.print("Sensor High(1):", LEFT, 210);
  myGLCD.printNumF(test1, 2, 245, 210);
}
bool InRange(float averageTempC) {
  if (averageTempC <= setTemp && averageTempC > (setTemp - 4)) {
    return true;
  } else return false;

}



void SendTemp(float currentTemp) {
  String sendString = "\'1\',\'" + String(rtc.getDateStr(FORMAT_BIGENDIAN, FORMAT_LONG, '-')) + " " + String(rtc.getTimeStr(FORMAT_LONG)) + "\',\'" + String(currentTemp) + "\'";
  if (InRange(currentTemp)) {
    sendString += ",\'1\'";
  } else {
    sendString += ",\'0\'";
  }
  //Serial.println(sendString);
  Serial2.println(sendString); //need to chamge to string w av temp, date and cooler ID (1)
  /*
    if (Serial2.available()) {
    Serial.println("Data NOT Sent");
    } else {
    Serial.print("Data Sent:");k
    Serial.println(currentTemp);
    }*/
}

// KEYPAD SELECT
// ISRs -- only count if signal is long enough
void selISR() {
  trigger = millis();
  if ((trigger - pressed) > 250) {
    myGLCD.setColor(VGA_BLACK);
    myGLCD.fillRect(0, 80, 500, 230);
    selectOn = !selectOn;
  }
  pressed = trigger;
}

void incISR() {
  trigger = millis();
  if (((trigger - pressed) > 250) && selectOn) setTemp++ ;
  
  pressed = trigger;
}

void decISR() {
  trigger = millis();
  if (((trigger - pressed) > 250) && selectOn) setTemp-- ;
  
  pressed = trigger;
}