//this is the first itteration of the ESP32 RECIEVER UART

#define RXp2 35
#define TXp2 13

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600,SERIAL_8N1, RXp2,TXp2);
}
void loop() {
  Serial.print("Data Recieved: ");
  Serial.println(Serial2.readString());
}