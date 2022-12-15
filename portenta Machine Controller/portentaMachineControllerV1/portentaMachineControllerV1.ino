 
/* 
https://github.com/arduino-libraries/Arduino_MachineControl/tree/master/examples
https://www.amritaryal.com.np/posts/categories/microcontrollers/3.html 
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
  
unsigned int CountDownTime = 30;
unsigned int ExperimentTime = 15;
unsigned int EndingSequense = 10;



void setup() {
  analog_out.period_ms(AnSeperator, 4);
  analog_out.period_ms(AnExternFan, 4);

  digital_outputs.setLatch();  
  //At startup set all channels to CLOSED
  digital_outputs.setAll(0);
  Serial.begin(9600); 
  while (Serial.available() == 0) {
  }   
}

void loop() {

  Serial.print("Input Voltage for Fan (V): ");
  while (Serial.available() == 0) {
  }
  int AnExternFanVoltage = Serial.parseInt();
  Serial.println(AnExternFanVoltage);
  delay(1000);

  Serial.print("Input Seperator (V): ");
  while (Serial.available() == 0) {
  }
  int AnSeperatorVoltage = Serial.parseInt();
  Serial.println(AnSeperatorVoltage);
  delay(1000);

  Serial.print("Input count down time (seconds): ");
  while (Serial.available() == 0) {
  }
  int CountDownTime = Serial.parseInt();
  Serial.println(CountDownTime);
  delay(1000);
  

  Serial.print("Input Experiment Time (seconds): ");  
  while (Serial.available() == 0) {
  }  
  int ExperimentTime = Serial.parseInt();
  Serial.println(ExperimentTime);

  delay(1000);


  Serial.print("Input Ending Sequense (seconds): ");
  while (Serial.available() == 0) {
  }    
  int EndingSequense = Serial.parseInt();
  Serial.println(EndingSequense);

  delay(100);

  Serial.println("======= DowSeq =======");
  countDown(CountDownTime);
  Serial.println("======================");

  // delay(2000);
    
  Serial.println("======= EndSeq =======");
  endingSeq(ExperimentTime, EndingSequense);
  Serial.println("======================");

  delay(3000);
}


void countDown(int max) {
  digital_outputs.set(led1, HIGH);
  Serial.println("Start led1");
  for(int i = max; i > 0; i--) {
    Serial.print(i);5

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
