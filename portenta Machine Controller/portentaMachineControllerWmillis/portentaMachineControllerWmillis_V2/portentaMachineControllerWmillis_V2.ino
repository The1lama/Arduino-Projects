 
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

int CountDownTime = 20000; // how long the count down is going to take // get time in seconds
int ExperimentTime = 15000; // the wait time between count down and end sequense // get time in seconds
int EndingSequense = 8000;   // how long time ending sequense is going to take // get time in seconds

int ownDelay = 8; // creates a delay after and operation of x ms

void setup() {
  analog_out.period_ms(AnSeperator, 4);
  analog_out.period_ms(AnExternFan, 4);

  digital_outputs.setLatch(); 
  //digital_inputs.set(); 
  //At startup set all channels to CLOSED
  digital_outputs.setAll(0);
  Serial.begin(9600);  
}

void loop() {

  long constStartMillis = millis();
  long EndOfCountdown = constStartMillis + CountDownTime ; 
  long ExperimentTimeMillis = constStartMillis + CountDownTime + ExperimentTime ; 
  long EndTimeMillis = constStartMillis + CountDownTime + ExperimentTime + EndingSequense ; 

  

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

    if ((currentMillis - constStartMillis) <= 500) {
      digital_outputs.set(led1, HIGH);
      Serial.println("Turned on led 1");
      delay(ownDelay);
    }

    if (currentMillis == StartFan) {
      Serial.print("Start Fan: ");
      Serial.print(StartFan);
      
      digital_outputs.set(DexternFan, HIGH);
      analog_out.write(AnExternFan, AnExternFanVoltage);
      Serial.println("\nStart Fan at "+String(AnExternFanVoltage)+"V");
      delay(ownDelay);
    }

    if (currentMillis == StartSeperator) {
      Serial.print("Start Seperator: ");
      Serial.print(StartSeperator);

      digital_outputs.set(seperator, HIGH);
      analog_out.write(AnSeperator, AnSeperatorVoltage);
      Serial.println("\nStart Seperator");
      delay(ownDelay);
    }

    if (currentMillis == StartCO2) {
      Serial.print("Start CO2: ");
      Serial.print(StartCO2);

      Serial.print(", Start fluid: ");
      Serial.print(Startfluid);
      
      digital_outputs.set(CO2Pin, HIGH);
      digital_outputs.set(fluidPump, HIGH);
      digital_outputs.set(led2, HIGH);
      Serial.println("\nStart CO2 & fluid pump & led2"); 
      delay(ownDelay);
    }

    if (currentMillis == EndOfCountdown) {
      Serial.print("End of countdown: ");
      Serial.println(EndOfCountdown);

      Serial.println();
      delay(ownDelay);
    }

  //=============Experiment Time==============================

    if (currentMillis == ExperimentTimeMillis) {
      Serial.print("Experiment Time: ");
      Serial.println(ExperimentTimeMillis);

      Serial.println();
      delay(ownDelay);
    }

  //=============End Time Sequense==============================

    if (currentMillis == EndCO2) {
      Serial.print("Stopping fluid: ");
      Serial.print(Endfluid);

      Serial.print(", Stopping CO2: ");
      Serial.print(EndCO2);

      digital_outputs.set(fluidPump, LOW);
      digital_outputs.set(CO2Pin, LOW);
      digital_outputs.set(led2, LOW);
      Serial.println("\nStoped fluid & CO2 & led2 ");
      delay(ownDelay);
    }

    if (currentMillis == EndSeperator) {
      Serial.print("Stopping separator: ");
      Serial.print(EndSeperator);      
      
      digital_outputs.set(seperator, LOW);
      analog_out.write(AnSeperator, 0);
      Serial.println("\nStoped Separator");
      delay(ownDelay);
    }

    if (currentMillis == EndFan) {
      Serial.print("Stopping Fan: ");
      Serial.print(EndFan);      
      
      digital_outputs.set(DexternFan, LOW);
      analog_out.write(AnExternFan, 0);
      digital_outputs.set(led1, LOW);
      Serial.println("\nStoped Extern Fan & Turn off led1 ");  
      delay(ownDelay);
    }

    if (currentMillis == EndTimeMillis) {
      Serial.print("End Time: ");
      Serial.println(EndTimeMillis);
      delay(ownDelay);      
    }

    if (currentMillis == (EndTimeMillis + 5000)) {
      break;
    }

  }
}
