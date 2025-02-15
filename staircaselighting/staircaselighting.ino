  
int ledPinOpp=13;
int ledPinNed=12;
int pirPinOpp=2;
int pirPinNed=3;
int pirStatOpp=0;
int pirStatNed=0;  
  
void setup() {
pinMode(ledPinOpp,OUTPUT);
pinMode(ledPinNed,OUTPUT);

pinMode(pirPinOpp,INPUT);
pinMode(pirPinNed,INPUT);

Serial.begin(9600);
}

void loop() {
  pirStatOpp=digitalRead(pirPinOpp);
  pirStatNed=digitalRead(pirPinNed);

  if (pirStatOpp== HIGH) {
    digitalWrite(ledPinOpp,HIGH);
    Serial.println("Så deg opp");
    delay(100);
  }
  else {
    digitalWrite(ledPinOpp,LOW);
    Serial.println("Så deg ikke Opp");
    delay(100);
  }

  if (pirStatNed== HIGH) {
    digitalWrite(ledPinNed,HIGH);
    Serial.println("Så deg ned");
    delay(100);
  }
  else {
    digitalWrite(ledPinNed,LOW);
    Serial.println("Så deg ikke ned");
    delay(100);
  }


}
