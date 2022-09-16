#include <SPI.h>
#include <SD.h>
#include <OneWire.h>
#include <DallasTemperature.h>


const int oneWireBus = 4; // GPIO where the DS18B20 is connected to    
OneWire oneWire(oneWireBus);// Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature sensor

#define SD_SC_PIN 5
int count =1;
int scount = 0;

void setup() {
  
  Serial.begin(115200);
  sensors.begin(); // Start the DS18B20 sensor
  Serial.print("Initializing SD card...");
  
  if (!SD.begin(SD_SC_PIN)) 
    {
      Serial.println("Card failed, or not present");
      while (1);
    }
  
  Serial.println("card initialized.");

  SPI.setClockDivider(SPI_CLOCK_DIV128);

  uint8_t cardType = SD.cardType();
  
  if(cardType == CARD_NONE)
    {
      Serial.println("No card attached");
    return;
    }

  Serial.print("Card type: ");
    if(cardType == CARD_MMC)
    {
      Serial.println("MMC");
    } else if(cardType == CARD_SD){
      Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
      Serial.println("SDHC");
    } else {
      Serial.println("UNKNOWN");
    }
    
// Card Information
Serial.print("Card size (Mb): ");
Serial.println((int)(SD.cardSize()/1048576));
Serial.print(" Available (Mb):");
Serial.println((int)(SD.totalBytes()/1048576));
Serial.print(" Used (Mb):");
Serial.println((int)(SD.usedBytes()/1048576));
}

const char *fileName = ("/Test_write_", scount,".txt");

void loop() {
  Serial.println(scount);
  File dataFile = SD.open(fileName,FILE_APPEND); // OPENS AND APPENDS TO THE FILE
  //File dataFile = SD.open(fileName,FILE_READ); //OVERWRITES THE FILE
  //File dataFile = SD.open(fileName,FILE_READ); //READS THE FILE
  if(dataFile){
    Serial.print("Opened file:");
    Serial.println(fileName);
    
    while(count <3 || count == 3){
    sensors.requestTemperatures(); 
    float temperatureC = sensors.getTempCByIndex(0);
    float temperatureF = sensors.getTempFByIndex(0);
    //PRINTING TO SCREEN
    Serial.print(count);
    Serial.print(" :Celcius Temperature is ");
    Serial.print(temperatureC);
    Serial.println("ºC");
    // WRITING TO THE CARD
    dataFile.print(count);
    dataFile.print(" :Celcius Temperature is ");
    dataFile.print(temperatureC);
    dataFile.println("ºC");
    dataFile.close();
    delay(10000);
    count++;
    }
    //dataFile.close();
    Serial.println("data file closed");
    delay(10000);
    scount++;
    Serial.println(scount);
    exit(0);
  }else{
    Serial.print("Can't create file: "); 
    Serial.println(fileName);
  } 
  delay(1000);
  
}
