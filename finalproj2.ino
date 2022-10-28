// OLED libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// SDA 20, SDA 21
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// coin amounts
int dimes = 0;
int pennies = 0;
int nickels = 0;
int quarters = 0;
// interrupt variables (using millis())
volatile unsigned long int trigger = 0; // time when triggered
volatile unsigned long int pressed = 0; // time since triggered

void setup() {
  // interrupt for each IR sensor, read when low
  attachInterrupt(digitalPinToInterrupt(2),UpDime,LOW); // dimes
  attachInterrupt(digitalPinToInterrupt(3),UpPen,LOW); // pennies
  attachInterrupt(digitalPinToInterrupt(18),UpNick,LOW); // nickels
  attachInterrupt(digitalPinToInterrupt(19),UpQuart,LOW); // quarters
  
  Serial.begin(9600); // sync baud rate

  // INITIALIZE OLED DISPLAY
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed")); // print if can't connect to oled
    for(;;);
  }
  delay(2000); // time to initialize
  display.setTextColor(WHITE);
}

void loop() {
  // Format OLED display 
  /*Bea's Coin Counter
   * Pennies: 0
   * Nickels: 0
   * Dimes: 0
   * Quarters: 0
   * $0.00
   */
  // TITLE
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Bea's Coin Counter");
  display.display(); 

  // COIN COUNTS
  // PENNIES
  display.setCursor(0, 16); // move down 16 pixels
  display.print("Pennies: ");
  display.println(pennies);
  display.display(); 
  // NICKELS
  display.setCursor(0, 24);  // move down 24 pixels
  display.print("Nickels: ");
  display.println(nickels);
  display.display(); 
  // DIMES
  display.setCursor(0, 32);  // move down 32 pixels
  display.print("Dimes: ");
  display.println(dimes);
  display.display(); 
  // QUARTERS
  display.setCursor(0, 40);  // move down 40 pixels
  display.print("Quarters: ");
  display.println(quarters);
  display.display(); 
  // TOTAL IN DOLLARS
  display.setTextSize(2); // bigger text size
  display.setCursor(0, 48);  // move down 48 pixels
  display.print("$");
  // calculating total, outputs float
  display.println((quarters * 0.25) + (dimes * .10) + (nickels * 0.05) + (pennies*0.01));
  display.display(); 
  // print every 3 secs
  delay(2000);
}

// ISR's
// inc. dime count
void UpDime() {
 trigger = millis();
 // only count if signal is long enough
 if ((trigger - pressed) > 250){
  dimes++;
 }
 pressed = trigger; // update time since triggered
}
// inc. penny count
void UpPen() {
  trigger = millis();
  // only count if signal is long enough
 if ((trigger - pressed) > 250){
  pennies++;
 }
 pressed = trigger; // update time since triggered
}
// inc. nickel count
void UpNick() {
  trigger = millis();
  // only count if signal is long enough
 if ((trigger - pressed) > 250){
  nickels++;
 }
 pressed = trigger; // update time since triggered
}
// inc. quarter count
void UpQuart() {
  trigger = millis();
  // only count if signal is long enough
 if ((trigger - pressed) > 250){
  quarters++;
 }
 pressed = trigger; // update time since triggered
}
