// WIFI AND MQTT
#include <WiFi.h>
#include <PubSubClient.h>
// SD CARD
#include <SPI.h>
#include <SD.h>

// SD PIN
#define SD_SC_PIN 5

// UART
#define RXp2 35
#define TXp2 13

// LID PIN
#define lid 15

// WIFI CONNECTION
const char* ssid     = "teeth";
const char* password = "stardewv";

// MQTT BROKER
const char *mqtt_broker = "thermostore.info";
const char *mqtt_topic = "espCooler"; // espCooler
// const char *mqtt_username = "bea";
// const char *mqtt_password = "almonte";
const int mqtt_port = 1883;

// SETTING UP WIFI AND MQTT
WiFiClient espClient;
PubSubClient client(espClient);

String readUART(); // READ FROM ARDUINO
void sendUART(String UARTString); // SEND TO ARDUINO 

bool ConnectToNetwork(const char * ssid, const char * password); // CONNECT TO WIFI NETWORK
bool WifiConnected(); // WIFI CHECK
bool SendServer(String serverSend, const char * mqttTopic); // ATTEMPT TO SEND TO SERVER
void callback(char *topic, byte *payload, unsigned int length); // MQTT CALLBACK
bool MQTTConnect(const char * mqttBroker, const int mqttPort); // CONNECT TO MQTT

void InitializeSD(); // Initilize SD card
void WriteSD(String lineString, String filename); // ATTEMPT TO WRITE TO SD CARD
bool ReadSD(String filename); // ATTEMPT TO READ FROM SD CARD

void lidISR();

bool lidOpen;
String arduinoString;

int linecount = 0;
void setup() {
  // LID INTERRUPT SETUP
  /*
  pinMode(lid, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(lid),*/
  // LID PIN SETUP
  pinMode(lid, INPUT);
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600,SERIAL_8N1, RXp2,TXp2);

  // SETUP WIFI
  ConnectToNetwork(ssid,password);
  // CONNECT TO SERVER
  MQTTConnect(mqtt_broker, mqtt_port);
  // INITIALIZE SD
  InitializeSD();
  //ReadSD("/test2.txt");
}

void loop() {

  readUART();
  if (linecount == 3){
    ReadSD("/test3.txt");
  }
}

// CONNECT TO NETWORK
bool ConnectToNetwork(const char * ssid, const char * password) {
    int timeout = 0;
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED && timeout <= 30) {
        delay(500);
        Serial.print(".");
        timeout++;
    }
    // SEND ERROR IF TIMEOUT
    if (timeout <= 30) {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        WiFi.mode(WIFI_STA);
        return true;
    } else {
        // SEND UART ERROR
        sendUART("10");
        return false;
    }

}

// READ FROM ARDUINO
String readUART() {
    String arduinoString;
    if (Serial2.available()) {
        arduinoString = Serial2.readString();
        Serial.print("Arduino: ");
        Serial.print(arduinoString);
        if (arduinoString.length() > 30 &&arduinoString.length() < 50) {
          linecount++;
          WriteSD(arduinoString,"/test3.txt");
        } else {
          Serial.println("Invalid String Input");
        }
    }
    return arduinoString;
}

// SEND TO ARDUINO
void sendUART(String UARTString) {
    if (Serial2.available()) {
        Serial2.println(UARTString);
    }
}

// WIFI CHECK
bool WifiConnected() {
    if (WiFi.status() == WL_CONNECTED) {
        return true;
    } else {
        return false;
    }
}

// OPEN SD
void InitializeSD() {
    Serial.print("Initializing SD card...");
  
    while (!SD.begin(SD_SC_PIN)) 
    {
      Serial.println("Card failed, or not present");
      delay(2000); // wait 2 secs and reinitialize
    }
  
    Serial.println("Card initialized.");

    SPI.setClockDivider(SPI_CLOCK_DIV128);

    uint8_t cardType = SD.cardType();
  
    if(cardType == CARD_NONE) {
      Serial.println("No card attached");
      return;
    }

    Serial.print("Card type: ");
    if(cardType == CARD_MMC) {
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


// ATTEMPT TO WRITE TO SD CARD
// fileName = /finalTest.txt
void WriteSD(String lineString, String fileName) {
    File dataFile = SD.open(fileName,FILE_APPEND); // OPENS AND APPENDS TO THE FILE
    
    if (digitalRead(lid) == LOW) {
      // opened lid
      lineString += ",\'1\'";
   } else {
      // closed lid
      lineString +=  ",\'0\'";
   }
    
    if(dataFile){
        Serial.print("Opened file: ");
        Serial.println(fileName);
        Serial.print("Saving: ");
        Serial.print(lineString);
        dataFile.print(lineString);
        
        dataFile.close();
    } else {
        Serial.print("Can't Append: "); 
        Serial.println(fileName);
        sendUART("12");
        InitializeSD();
        WriteSD(lineString, fileName);
  } 
}

// ATTEMPT TO READ FROM SD CARD
bool ReadSD(String filename) {
   File dataFile = SD.open(filename,FILE_READ); // OPENS AND APPENDS TO THE FILE
   String fileInput;
   int lineSentCount = 0;
   while (dataFile.available()) {
      fileInput = "";
      while (dataFile.peek() != '\n') {
          if (dataFile.peek() > 31) {
            fileInput += char(dataFile.read());
            
          } else {
            dataFile.read();
          }
          
      }
      if (dataFile.peek() == '\n')
        dataFile.read();
      //Serial.println();
      if (fileInput.length() > 30 && fileInput.length() < 50) {
        lineSentCount++;
        delay(1000);
        SendServer(fileInput, mqtt_topic);
        Serial.print("Read and Sent: ");
        Serial.println(fileInput);
      }  
   }
   dataFile.close();
   // delete file after sending
   // try file write again, and remove (/test.txt)
   dataFile = SD.open(filename,FILE_WRITE);
   if (dataFile.available()) {
      dataFile.print("");
   }
   linecount = 0;
   dataFile.close();
   

}

// ATTEMPT TO SEND TO SERVER
bool SendServer(String serverSend, const char * mqttTopic) {
    const char * mqttString = serverSend.c_str();
    if (client.publish(mqttTopic,mqttString)) {
        return true;
    } else return false;
}

// MQTT CALLBACK
void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

// CONNECT TO MQTT BROKER
bool MQTTConnect(const char * mqttBroker, const int mqttPort) {
    int timeout = 0;
    
    client.setServer(mqttBroker, mqttPort);
    client.setCallback(callback);
    
    while (!client.connected() && timeout <= 15) {
        // Setting esp client ID
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        
        // Attempting to connect to broker
        if (client.connect(client_id.c_str())) {
            Serial.println("Connected to MQTT Broker");
            return true;
        } else {
            Serial.print("Failed with state ");
            Serial.println(client.state());
            delay(2000);
            timeout++;
        }
     }
     
   if (timeout > 15) {
        // SEND UART ERROR
        sendUART("11");
        return false;
   } else {
        return true;
   }
    
}
