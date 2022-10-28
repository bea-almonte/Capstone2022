// keypad code 

//ground on keyoad is 1
#define next 9//connect next key to pin 9
#define bef 10//connect before key to pin 10
#define inc 11 //connect increment key to pin 11
#define dec 12//connect decrement key to pin 12
#define sel 13 //conect select key to pin 13


void setup() {
  Serial.begin(9600);
  pinMode(sel, INPUT_PULLUP); //set pin as input
  pinMode(inc, INPUT_PULLUP); //set pin as input
  pinMode(dec, INPUT_PULLUP); //set pin as input
  pinMode(next, INPUT_PULLUP); //set pin as input
  pinMode(bef, INPUT_PULLUP); //set pin as input

}

void loop() {
 int selstatus = digitalRead(sel); //read if sel is pressed
 int incstatus = digitalRead(inc); //read if inc is pressed
 int decstatus = digitalRead(dec); //read if dec is pressed
 int nextstatus = digitalRead(next); //read if next is pressed
 int befstatus = digitalRead(bef); //read if bef is pressed

   if(!selstatus){
    Serial.println("select is pressed");
   }
 if(!incstatus){
    Serial.println("increment is pressed");
   }
    if(!decstatus){
    Serial.println("decrement is pressed");
   }
    if(!nextstatus){
    Serial.println("next is pressed");
   }
    if(!befstatus){
    Serial.println("before is pressed");
   }
   delay(300);
}