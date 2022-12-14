
#include <Arduino_MachineControl.h>

using namespace machinecontrol;

void setup() {
  // put your setup code here, to run once:
  digital_outputs.setLatch();  
  //At startup set all channels to CLOSED
  digital_outputs.setAll(0);
  Serial.begin(9600); 

}

void loop() {
  while (Serial.available() == 0) {
  }
  // put your main code here, to run repeatedly:
  Serial.println("\n1. Turn on Digital Leds");
  Serial.println("2. Turn on Analog Leds");
  Serial.println("3. Turn off leds");

  Serial.println("Which choise would you like to print? ");

  int menuChoice = Serial.parseInt();

  Serial.println("Another input test for print");
  char anotherstr = Serial.read();

  switch (menuChoice) {
    case 1:
      Serial.println("\nDigital leds on");
      digital_outputs.set(3, HIGH);
      digital_outputs.set(4, HIGH);

      break;
    
    case 2:
      Serial.println("World");
      break;
    
    case 3:
      Serial.println("!");
      digital_outputs.set(3, LOW);
      digital_outputs.set(4, LOW);
      break;
    
    default:
      Serial.println("Please choose a valid selection");
  }
  Serial.println(anotherstr);

}
