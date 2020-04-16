// Test file to move motor back and forth

// Defines pins numbers. Change to your appropriate pins
const int stepPin = 5; 
const int dirPin = 22; 
const int enPin = 28;

void setup() {
  
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,LOW);

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 200; x++) {
    digitalWrite(stepPin,HIGH); 
    delay(1); 
    digitalWrite(stepPin,LOW); 
    delay(1); 
  }
  delay(1000); // One second delay
  digitalWrite(LED_BUILTIN, HIGH);

  digitalWrite(dirPin,LOW); //Changes the rotations direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 200; x++) {
    digitalWrite(stepPin,HIGH);
    delay(1);
    digitalWrite(stepPin,LOW);
    delay(1);
  }
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
}
