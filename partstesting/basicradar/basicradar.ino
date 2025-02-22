
//Simple implementation of a radar

// Set used pins
const int motionSensorPin = D1;
const int LEDPin = D2;

//Set variables
int motionState;

void setup() {
  Serial.begin(9600);
  //Define LED and make sure its off to begin with, and set radar as input
  pinMode(motionSensorPin, INPUT);
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, LOW);
}

void loop() {
  //Read value of radar
  motionState = digitalRead(motionSensorPin);
  Serial.println(motionState);

  //Based on radar response turn on or off a LED and print the state in the serial
  if(motionState == 1){
    digitalWrite(LEDPin, HIGH);
    Serial.println("LED on!");
  }
  else{
    digitalWrite(LEDPin, LOW);
    Serial.println("LED off!");
  }
  delay (1000);
}