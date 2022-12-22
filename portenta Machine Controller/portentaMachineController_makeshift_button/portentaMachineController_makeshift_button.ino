/*
  Maximum input voltage is 10V.
  To use the 0V-10V functionality, a 24V supply on
*/
#include <Arduino_MachineControl.h>

using namespace machinecontrol;

float res_divider = 0.28057;
float reference = 3.3;
float counter = 5.5;

void setup() {
  analogReadResolution(16);
  analog_out.period_ms(0, 4);
  Serial.begin(9600);
  analog_in.set0_10V();

  digital_outputs.setLatch();
  digital_outputs.setAll(0);

  analog_out.write(3, counter);
  digital_outputs.set(7, HIGH);
}

void loop() {


  float raw_voltage_ch0 = analog_in.read(0);
  float voltage_ch0 = (raw_voltage_ch0 * reference) / 65535 / res_divider;
  Serial.print("Voltage CH0: ");
  Serial.print(voltage_ch0, 3);
  Serial.println("V");

  float raw_voltage_ch1 = analog_in.read(1);
  float voltage_ch1 = (raw_voltage_ch1 * reference) / 65535 / res_divider;
  Serial.print("Voltage CH1: ");
  Serial.print(voltage_ch1, 3);
  Serial.println("V");

  float raw_voltage_ch2 = analog_in.read(2);
  float voltage_ch2 = (raw_voltage_ch2 * reference) / 65535 / res_divider;
  Serial.print("Voltage CH2: ");
  Serial.print(voltage_ch2, 3);
  Serial.println("V");
  Serial.println();

  if (voltage_ch2 > 4) {
    Serial.println("The Button is on");
  }
  else {
    Serial.println("");
  }
  Serial.println();
  delay(250);
}
