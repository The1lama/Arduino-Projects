
#include <Arduino_MachineControl.h>

using namespace machinecontrol;


const int led1 = 0; // Led on, Digital pin
const int led2 = 5;    // led on , Digital pin 

const int DexternFan = 1;  // extern fan, Digital pin
const int seperator = 2;  // seperator, Digital pin
const int CO2Pin = 3;     // CO2-flow, Digital pin
const int fluidPump = 4;  // fluid pump, Digital pin

const int AnExternFan = 0; // extern fan, Analog pin
float AnExternFanVoltage = 7.50; // Maximum ouput value 10.4V
const int AnSeperator = 2; // extern fan, Analog pin
float AnSeperatorVoltage = 2.5; // Maximum ouput value 10.4V

int oneMinute = 3; // timer for shuting down separator



unsigned long prevTime_T1 = millis();
unsigned long prevTime_T2 = millis();

long interval_CountDownTime = 20000;
long interval_ExperimentTime = 15000;
long interval_EndingSequense = 8000;


void setup() {
  analog_out.period_ms(AnSeperator, 4);
  analog_out.period_ms(AnExternFan, 4);

  digital_outputs.setLatch();  
  digital_outputs.setAll(0);  //At startup set all channels to CLOSED

  Serial.begin(9600);  
}

void loop() {
  unsigned long currentTime = millis();

      
  if (currentTime - prevTimeT1 > interval_CountDownTime) {
    if 
  }
}

void countDown(int max) {
  digital_outputs.set(led1, HIGH);
  Serial.println("Start led1");
  for(int i = max; i > 0; i--) {
    Serial.print(i);

    if (i == (max / 2)) {
      digital_outputs.set(DexternFan, HIGH);
      analog_out.write(AnExternFan, AnExternFanVoltage);
      Serial.println("\nStart Fan at "+String(AnExternFanVoltage)+"V"); 
    } 
    
    if (i == ((max / 2)- 5)) {
      digital_outputs.set(seperator, HIGH);
      analog_out.write(AnSeperator, AnSeperatorVoltage);
      Serial.println("\nStart Seperator");
    } 

    if (i == 1) {
      digital_outputs.set(CO2Pin, HIGH);
      digital_outputs.set(fluidPump, HIGH);
      digital_outputs.set(led2, HIGH);
      Serial.println("\nStart CO2 & fluid pump & led2"); // add later for code to turn off this thing
    }
    Serial.print(",");
    delay(1000);
  }
}

void endingSeq(int ExperimentTime, int EndingSequense) {
  int delayTimer = (ExperimentTime + EndingSequense) + 1;  
  for(int i = 1; i < (delayTimer); i++) {
    Serial.print(i);

    if (i == ExperimentTime) {
      digital_outputs.set(fluidPump, LOW);
      digital_outputs.set(CO2Pin, LOW);
      digital_outputs.set(led2, LOW);
      Serial.println("\nStoped fluid & CO2 & led2 ");
      }

    if (i == (delayTimer - oneMinute)) {
      digital_outputs.set(seperator, LOW);
      analog_out.write(AnSeperator, 0);
      Serial.println("\nStoped Separator");
    }
    Serial.print(",");
 
    delay(1000);
  }
  
  digital_outputs.set(DexternFan, LOW);
  analog_out.write(AnExternFan, 0);
  digital_outputs.set(led1, LOW);

  Serial.println("\nFinnished");
  Serial.print("Stoped Extern Fan & Turn off led1 ");  
}
