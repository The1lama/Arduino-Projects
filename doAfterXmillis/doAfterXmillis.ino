
unsigned long previousMillis = 0; // last time update
int CountDownTime = 20000; // how long the count down is going to take // get time in seconds
int ExperimentTime = 15000; // the wait time between count down and end sequense // get time in seconds
int EndingSequense = 8000;   // how long time ending sequense is going to take // get time in seconds

long interval = (int(millis()) + 20000);// long interval = (int(millis()) + 20000); // interval at which to do something (milliseconds)

void setup(){
  // while (Serial.available() == 0) {
  // }
}

void loop(){
 
  unsigned long currentMillis = millis();
  Serial.println(currentMillis);

  if( (currentMillis - previousMillis) > interval && (currentMillis - previousMillis) < interval+1000 ) {
    previousMillis = currentMillis;  
    Serial.println("Yhea after 20 seconds from the countdown");
     // do something
  }

  delay(1000);
  Serial.println("");
}