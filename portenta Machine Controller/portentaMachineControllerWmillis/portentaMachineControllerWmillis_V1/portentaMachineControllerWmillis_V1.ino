 
/* 
      To Do 

inputs for the custom rules
  Voltage Value

  waiting time
    Nedräkning
    exprerimenttid
    avslutstid

create some button or something to start the code prossess
 */

#include <Arduino_MachineControl.h>

using namespace machinecontrol;

const int led1 = 0; // Led for machine on, Digital pin
const int led2 = 5;    // wait time led, Digital pin 

const int DexternFan = 1;  // extern fan, Digital pin
const int seperator = 2;  // seperator, Digital pin
const int CO2Pin = 3;     // CO2-flow, Digital pin
const int fluidPump = 4;  // fluid pump, Digital pin

const int AnExternFan = 1; // extern fan, Analog pin
float AnExternFanVoltage = 7.50; // Maximum ouput value 10.4V
const int AnSeperator = 2; // extern fan, Analog pin
float AnSeperatorVoltage = 2.5; // Maximum ouput value 10.4V

int oneMinute = 3; // timer for shuting down separator

int CountDownTime = 20000; // how long the count down is going to take // get time in seconds
int ExperimentTime = 15000; // the wait time between count down and end sequense // get time in seconds
int EndingSequense = 8000;   // how long time ending sequense is going to take // get time in seconds


void setup() {
  analog_out.period_ms(AnSeperator, 4);
  analog_out.period_ms(AnExternFan, 4);

  digital_outputs.setLatch();  
  //At startup set all channels to CLOSED
  digital_outputs.setAll(0);
  Serial.begin(9600);  
}

void loop() {
  const long constStartMillis = millis();

  long EndOfCountdown = constStartMillis + CountDownTime ; 
  long ExperimentTimeMillis = constStartMillis + CountDownTime + ExperimentTime ; 
  long EndTimeMillis = constStartMillis + CountDownTime + ExperimentTime + EndingSequense ; 


  Serial.println(constStartMillis);
  Serial.println(EndOfCountdown);


  while(true) {
    long currentMillis = millis();

  // combined millis to end
    long EndOfCountdown = constStartMillis + CountDownTime ; 
    long ExperimentTimeMillis = constStartMillis + CountDownTime + ExperimentTime ; 
    long EndTimeMillis = constStartMillis + CountDownTime + ExperimentTime + EndingSequense ; 

  // Start sequense varibles
    long StartFan = constStartMillis + (CountDownTime / 2);
    long StartSeperator = constStartMillis + ((CountDownTime / 2) + 5000);
    long StartCO2 = constStartMillis + (CountDownTime - 1000);
    long Startfluid = constStartMillis + (CountDownTime - 1000);

  // End sequense Varibles
    long EndFan = EndTimeMillis ;
    long EndSeperator = EndTimeMillis - (EndingSequense / 2);
    long EndCO2 = ExperimentTimeMillis ;
    long Endfluid = ExperimentTimeMillis ;


  //=============Countdown Sequense==============================

    Serial.print("Start Millis: ");
    Serial.println(constStartMillis);

    Serial.print("Current Millis: ");
    Serial.println(CurrentMillis);
    Serial.println();

    Serial.print("Start Fan: ");
    Serial.println(StartFan);

    Serial.print("Start Seperator: ");
    Serial.println(StartSeperator);

    Serial.print("Start CO2: ");
    Serial.println(StartCO2);

    Serial.print("Start fluid: ");
    Serial.println(Startfluid);

    Serial.print("End of countdown: ");
    Serial.println(EndOfCountdown);

    Serial.println();

  //=============Experiment Time==============================

    Serial.print("Experiment Time: ");
    Serial.println(ExperimentTimeMillis);

    Serial.println();
    
  //=============End Time Sequense==============================

    Serial.print("Stopping fluid: ");
    Serial.println(Endfluid);

    Serial.print("Stopping CO2: ");
    Serial.println(EndCO2);

    Serial.print("Stopping separator: ");
    Serial.println(EndSeperator);

    Serial.print("Stopping Fan: ");
    Serial.println(EndFan);

    Serial.print("End Time: ");
    Serial.println(EndTimeMillis);
    
  //===========================================
    
    Serial.println();
    delay(1000);
  }
}




