
int pirPinUp=2;
int pirPinDown=3;
bool pirStatUp=false;
bool pirStatNed=false;  
  
int LEDPins[] = {12,13};
int numberOfElements = sizeof(LEDPins) / sizeof(LEDPins[0]);
void setup() {
pinMode(12,OUTPUT);
pinMode(13,OUTPUT);

pinMode(pirPinUp,INPUT);
pinMode(pirPinDown,INPUT);

Serial.begin(9600);
Serial.println(numberOfElements);
}

void loop() {
  pirStatUp=digitalRead(pirPinUp);
  delay(1000);
  //pirStatDown=digitalRead(pirPinNDown);
  Serial.print("Value before if: ");
  Serial.println(pirStatUp);

  if (pirStatUp == HIGH){
  //Serial.println(numberOfElements);
  delay(500);
     for(int i = 0; i < numberOfElements ; i++){
      digitalWrite(LEDPins[i] ,HIGH);
      Serial.println("Active up");
       delay(1000);
    }

  }
  else{
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);
    delay(1000);
  }
 
Serial.println("Round over");
}

