
const int pushButton = 2; // on digital pin 2
const int digitalPin = 0; // digital out pin 0 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  // Makes the pushbutton in put 
  pinMode(pushButton, INPUT);

  pinMode(digitalPin, OUTPUT);
  digitalWrite(digitalPin, LOW);

}

void loop() {
  Serial.println("===========================");

  // reads input of pin
  int buttonState = digitalRead(pushButton);
  int pinState = digitalRead(digitalPin);

  // an if statement
  if (pinState == 0){
    Serial.println("Off");
    digitalWrite(digitalPin, HIGH);
    Serial.println("Turn pinState to HIGH");
    Serial.println(pinState);

  } else{
    Serial.println("On");
    digitalWrite(digitalPin, LOW);
    Serial.println("Turn pinState to LOW");
    Serial.println(pinState);
    
    }
  Serial.println("===========================");

  delay(2000); // wait time

}
