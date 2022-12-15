
#include <Arduino_MachineControl.h>

using namespace machinecontrol;


// previous time for the tasks depending upon time.
unsigned long prevTime_T1 = millis(); 
unsigned long prevTime_T4 = millis(); 

// time intervals for the tasks
long interval_T1 = 1000; // blink every 1 second
long interval_T4 = 5000; // print brightness of LED3 every 5 seconds

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Set over current behavior of all channels to latch mode:
  digital_outputs.setLatch();
  digital_outputs.setAll(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentTime = millis();

  // Task 1 : Blink LED1 (T1)
  if (currentTime - prevTime_T1 > interval_T1) {
    digital_outputs.set(1, HIGH);
    Serial.print("CH1 is CLOSED for 1 s...");
    delay(1000);
    digital_outputs.set(1, LOW);
    Serial.println("now is OPEN.");

    prevTime_T1 = currentTime;
  }

  // Task 4 : print the brightness of LED3 in the serial monitor after every 5 seconds
  if (currentTime - prevTime_T4 > interval_T4) {
    digital_outputs.set(5, HIGH);
    Serial.print("CH5 is CLOSED for 1 s...");
    delay(1000);
    digital_outputs.set(5, LOW);
    Serial.println("now is OPEN.");

    prevTime_T4 = currentTime;
  }
}