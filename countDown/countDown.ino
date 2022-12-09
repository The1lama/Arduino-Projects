

void setup() {

}

void loop() {

  int CountDownTime = 10; // get time in seconds
  int ExperimentTime = 4; // get time in seconds
  int EndingTimer = 10;   // get time in seconds

  // countDown(CountDownTime);

  // delay(ExperimentTime);

  countUp();

}


void countDown(int max) {

  for(int i = max; i > 0; i--) {
    Serial.print(i);
    Serial.print(", ");
    if (i == (max / 2)) {
      Serial.print("Bruh, ");
    } 
    if (i == (max / 4)) {
      Serial.print("Something else, ");
    } 
    delay(1000);
  }
  Serial.println("GO ... ");
}

void countUp() {
    for (int i = 0; i < 10; i++) {
        Serial.println(i);
        delay(1000);
    }
}