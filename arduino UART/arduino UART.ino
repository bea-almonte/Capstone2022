//this is the first itteration of the ARDUINO SENDER UART
void setup() {
  // put your setup code here, to run once:
  Serial2.begin(9600);
  Serial.begin(9600);
}
void loop() {
  Serial2.println("Test 2"); //need to chamge to string w av temp, date and cooler ID (1)
  if(Serial2.available()){
    Serial.println("Data NOT Sent");
  delay(1500);
  }
  else{
    Serial.println("Data Sent");
  delay(1500);
  }
  
  
}