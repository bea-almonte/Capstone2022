int Relay1 =2;
int Relay2 =3;
int D =1000;

void setup() {
  pinMode(Relay1,OUTPUT);
  pinMode(Relay2,OUTPUT);

  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);

}

void loop() {
  digitalWrite(Relay1,LOW);
  delay(D);
  digitalWrite(Relay1, HIGH);
  delay(D);
  digitalWrite(Relay2,LOW);
  delay(D);
  digitalWrite(Relay2, HIGH);
  delay(D);
}
