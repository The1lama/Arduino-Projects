/****************************************************************************************************************************
  Async_PostServer.ino

  For Portenta_H7 (STM32H7) with Vision-Shield Ethernet or Murata WiFi

  Portenta_H7_AsyncWebServer is a library for the Portenta_H7 with Vision-Shield Ethernet or Murata WiFi

  Based on and modified from ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
  Built by Khoi Hoang https://github.com/khoih-prog/Portenta_H7_AsyncWebServer
  Licensed under GPLv3 license

  ord code: https://github.com/khoih-prog/Portenta_H7_AsyncWebServer/blob/main/examples/WiFi/Async_PostServer/Async_PostServer.ino 

 *****************************************************************************************************************************/

#if !( defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) )
	#error For Portenta_H7 only
#endif

#define _PORTENTA_H7_AWS_LOGLEVEL_     1

#define USE_WIFI_PORTENTA_H7        true

#include <WiFi.h>
#warning Using WiFi for Portenta_H7.

#include <Portenta_H7_AsyncWebServer.h>

#include <Arduino_MachineControl.h>

using namespace machinecontrol;

  ///////////////////////////////////

char ssid[] = "";        // your network SSID (name)
char pass[] = "";        // your network password (use for WPA, or use as key for WEP), length must be 8+

int status = WL_IDLE_STATUS;      // idk

int CountDownTimePara = 1;        // get time in ms
int ExperimentTimePara = 1;       // get time in ms
int EndingSequensePara = 1;       // get time in ms
float AnExternFanVoltPara = 7.5;  // Maximum ouput value 10.4V
float AnSeperatorVoltPara = 7.5;  // Maximum ouput value 10.4V

  ///////////////////////////////////

int machineON = 0;            // to be a switch variable for turning on the sequense for the machine

const int led1 = 0;          // Led for machine on,  Digital pin
const int led2 = 5;         // wait time led,        Digital pin 

const int DexternFan = 1;   // extern fan,           Digital pin
const int seperator = 2;    // seperator,            Digital pin
const int CO2Pin = 3;       // CO2-flow,             Digital pin
const int fluidPump = 4;    // fluid pump,           Digital pin

const int AnExternFan = 1; // extern fan,            Analog pin
const int AnSeperator = 2; // extern fan,            Analog pin

float AnExternFanVoltage = 7.50; // Maximum ouput value 10.4V 
float AnSeperatorVoltage = 2.5; // Maximum ouput value 10.4V

int CountDownTime = 0;  // how long the count down is going to take           // get time in ms
int ExperimentTime = 0; // the wait time between count down and end sequense  // get time in ms
int EndingSequense = 0; // how long time ending sequense is going to take     // get time in ms

int ownDelay = 8; // creates a delay after and operation of x ms

///////////////////////////////////  

AsyncWebServer    server(80);   // sets webserver port to 80

const String postForms =
"<html>\
  <head>\
    <title>Varible changer</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>CountDown Time</h1><br>\
    <form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/CountDownTime/\">\
      <input type=\"text\" name=\"CountDown_Time\" value=\"1\">\
      <input type=\"submit\" value=\"Submit\">  ms\
    </form>\
    <h1>Experiment Time</h1><br>\
    <form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/ExperimentTime/\">\
      <input type=\"text\" name=\"Experiment_Time\" value=\"1\">\
      <input type=\"submit\" value=\"Submit\">  ms\
    </form>\
    <h1>EndingSequense</h1><br>\
    <form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/EndingSequense/\">\
      <input type=\"number\" name=\"Ending_Sequense\" value=\"1\">\
      <input type=\"submit\" value=\"Submit\">  ms\
    </form>\
    <h1>AnExternFan Voltage</h1><br>\
    <form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/AnExternFanVoltage/\">\
      <input type=\"text\" name=\"AnExternFan_Voltage\" value=\"7.5\">\
      <input type=\"submit\" value=\"Submit\">  Max 10.4V\
    </form>\
    <h1>AnSeperator Voltage</h1><br>\
    <form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/AnSeperatorVoltage/\">\
      <input type=\"text\" name=\"AnSeperator_Voltage\" value=\"7.5\">\
      <input type=\"submit\" value=\"Submit\"> Max 10.4V\
    </form>\
    <h1>Buttons</h1>\
      <p>\    
        <a href=\"/runMachine\">\
          <button class=\"button\">runMachine</button>\
        </a>\ 
        <a href=\"/CusomVariable\">\
          <button class=\"button\">CusomVariable</button>\
        </a>\
      </p>\
  </body>\
</html>";

void handleRoot(AsyncWebServerRequest *request) {
	request->send(200, "text/html", postForms);
}

void handlePlain(AsyncWebServerRequest *request) {
	if (request->method() != HTTP_POST)
	{
		request->send(405, "text/plain", "Method Not Allowed");
	}
	else
	{
		request->send(200, "text/plain", "POST body was:\n" + request->arg("plain"));
	}
}

void handleForm(AsyncWebServerRequest *request) {
	if (request->method() != HTTP_POST)
	{
		request->send(405, "text/plain", "Method Not Allowed");
	}
	else
	{
		String message = "POST form was:\n";

		for (uint8_t i = 0; i < request->args(); i++) {
      String argName = request->argName(i);
      String arg = request->arg(i);

      message += " " + argName + ": " + arg + "\n";

      if (argName == "CountDown_Time") {
        Serial.print("CountDown Time is writen with value: ");
        Serial.println(arg);
        CountDownTimePara = arg.toInt();      // converts string to int
      } 

      if (argName == "Experiment_Time") {
        Serial.print("Experiment time is writen with value: ");
        Serial.println(arg);
        ExperimentTimePara = arg.toInt();     // converts string to int
      } 

      if (argName == "Ending_Sequense") {
        Serial.print("Ending Sequense is writen with value: ");
        Serial.println(arg);
        EndingSequensePara = arg.toInt();     // converts string to int
      } 

      if (argName == "AnExternFan_Voltage") {
        Serial.print("AnExternFanVoltage is writen with value: ");
        Serial.println(arg);
        AnExternFanVoltPara = arg.toFloat();  // converts string to float
      }

      if (argName == "AnSeperator_Voltage") {
        Serial.print("AnSeperatorVoltage is writen with value: ");
        Serial.println(arg);
        AnSeperatorVoltPara = arg.toFloat();  // converts string to float
      } 

		}
    Serial.println(message);
		request->send(200, "text/plain", message);
	}
}

void handleNotFound(AsyncWebServerRequest *request) {
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += request->url();
	message += "\nMethod: ";
	message += (request->method() == HTTP_GET) ? "GET" : "POST";
	message += "\nArguments: ";
	message += request->args();
	message += "\n";

	for (uint8_t i = 0; i < request->args(); i++)
	{
		message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
	}

	request->send(404, "text/plain", message);
}

void printWifiStatus() {  // prints wifi status
	// print the SSID of the network you're attached to:
	Serial.print("SSID: ");
	Serial.println(WiFi.SSID());

	// print your board's IP address:
	IPAddress ip = WiFi.localIP();
	Serial.print("Local IP Address: ");
	Serial.println(ip);

	// print the received signal strength:
	long rssi = WiFi.RSSI();
	Serial.print("signal strength (RSSI):");
	Serial.print(rssi);
	Serial.println(" dBm");
}

void variableInfo(AsyncWebServerRequest *request) { // page where to show the active varible numbers for the machine
	String message = "Custom variable info\n\n";
	message += "CountDownTime: ";
	message += CountDownTimePara;
	message += "\nExperimentTime: ";
	message += ExperimentTimePara;
	message += "\nEndingSequensePara: ";
	message += EndingSequensePara;
	message += "\nnExternFanVoltPara: ";
	message += AnExternFanVoltPara;
  message += "\nAnSeperatorVoltPara: ";
	message += AnSeperatorVoltPara;
	message += "\n";

	request->send(200, "text/plain", message);
}

void setup() { 
	Serial.begin(115200);

	while (!Serial && millis() < 5000);

	delay(200);

	Serial.print("\n===========================\nStart Async_PostServer on ");
	Serial.print(BOARD_NAME);
	Serial.print(" with ");
	Serial.println(SHIELD_TYPE);
	Serial.println(PORTENTA_H7_ASYNC_TCP_VERSION);
	Serial.println(PORTENTA_H7_ASYNC_WEBSERVER_VERSION);

	///////////////////////////////////

  analog_out.period_ms(AnSeperator, 4); // idk
  analog_out.period_ms(AnExternFan, 4); // idk

  digital_outputs.setLatch(); // enable digital outputs 
  digital_outputs.setAll(0);  // sets all digital outputs to 0 (closed)

	///////////////////////////////////

	// check for the WiFi module:
	if (WiFi.status() == WL_NO_MODULE)
	{
		Serial.println("Communication with WiFi module failed!");

		// don't continue
		while (true);
	}

	Serial.print(F("Connecting to SSID: "));
	Serial.println(ssid);

	status = WiFi.begin(ssid, pass);

	delay(1000);

	// attempt to connect to WiFi network
	while ( status != WL_CONNECTED)
	{
		delay(500);

		// Connect to WPA/WPA2 network
		status = WiFi.status();
	}

	printWifiStatus();

	///////////////////////////////////

	server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
		handleRoot(request);
	});

	server.on("/CusomVariable", [](AsyncWebServerRequest * request) {
    Serial.println("Open CustomVaribles");
    variableInfo(request);
	});

  server.on("/runMachine", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (machineON == 0) {
      Serial.println("Machine is Starting");
      int machineON = 1; 
      request->send(200, "text/html", "The Machine has begun (not currently working) ");      
    }
    else {
      Serial.println("Machine is allready going");
      request->send(200, "text/html", "Machine is allready going");
    }
  });

	//server.on("/CountDownTime/", handlePlain);
	server.on("/CountDownTime/", HTTP_POST, [](AsyncWebServerRequest * request) {
		handleForm(request);
	});

	//server.on("/ExperimentTime/", handleForm);
	server.on("/ExperimentTime/", HTTP_POST, [](AsyncWebServerRequest * request) {
		handleForm(request);
	});

	//server.on("/EndingSequense/", handleForm);
	server.on("/EndingSequense/", HTTP_POST, [](AsyncWebServerRequest * request) {
		handleForm(request);
	});

	//server.on("/AnExternFanVoltage/", handleForm);
	server.on("/AnExternFanVoltage/", HTTP_POST, [](AsyncWebServerRequest * request) {
		handleForm(request);
	});

	//server.on("/AnSeperatorVoltage/", handleForm);
	server.on("/AnSeperatorVoltage/", HTTP_POST, [](AsyncWebServerRequest * request) {
		handleForm(request);
	});

  ///////////////////////////////////

	server.onNotFound(handleNotFound);

	server.begin();

	Serial.print(F("HTTP Async_PostServer started @ IP : "));
	Serial.println(WiFi.localIP());

  ///////////////////////////////////
}

void heartBeatPrint() {
	static int num = 1;

	Serial.print(F("."));

	if (num == 80)
	{
		Serial.println();
		num = 1;
	}
	else if (num++ % 10 == 0)
	{
		Serial.print(F(" "));
	}
}

void paraPrint() {
  Serial.print("\nCountDownTime: ");
  Serial.println(CountDownTimePara);

  Serial.print("ExperimentTime: ");
  Serial.println(ExperimentTimePara);

  Serial.print("EndingSequense: ");
  Serial.println(EndingSequensePara);

  Serial.print("AnExternFanVoltage: ");
  Serial.println(AnExternFanVoltPara);

  Serial.print("AnSeperatorVoltage: ");
  Serial.println(AnSeperatorVoltPara);
}

void check_status() {
	static unsigned long checkstatus_timeout = 0;

 #define STATUS_CHECK_INTERVAL     10000L

	// Send status report every STATUS_REPORT_INTERVAL (60) seconds: we don't need to send updates frequently if there is no status change.
	if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0)){
		heartBeatPrint();
    // Serial.println(machineON); 
    // paraPrint();    
		checkstatus_timeout = millis() + STATUS_CHECK_INTERVAL;
	}
}

void machineRun() {
  float AnExternFanVoltage = AnExternFanVoltPara; // Maximum ouput value 10.4V 
  float AnSeperatorVoltage = AnSeperatorVoltPara; // Maximum ouput value 10.4V

  int CountDownTime = CountDownTimePara;   // get time in ms
  int ExperimentTime = ExperimentTimePara; // get time in ms
  int EndingSequense = EndingSequensePara; // get time in ms

  // Serial.println(CountDownTime);  
  if (machineON == 1) { // when machineON == 1 then one is enable to look for this part of the code
    Serial.println("Working machineON");
    delay(1000);
    long constStartMillis = millis();  // gets start millis when starting the code
    long EndOfCountdown = constStartMillis + CountDownTime ; // creates EndOfCountdown variable
    long ExperimentTimeMillis = constStartMillis + CountDownTime + ExperimentTime ; // creates ExperimentTimeMillis variable
    long EndTimeMillis = constStartMillis + CountDownTime + ExperimentTime + EndingSequense ; // creates EndTimeMillis variable

    while(true) {
      long currentMillis = millis();  // gets current Millis 

    // combined millis to end
      long EndOfCountdown = constStartMillis + CountDownTime ; // creates EndOfCountdown variable
      long ExperimentTimeMillis = constStartMillis + CountDownTime + ExperimentTime ; // creates ExperimentTimeMillis variable
      long EndTimeMillis = constStartMillis + CountDownTime + ExperimentTime + EndingSequense ; // creates EndTimeMillis variable

    // Start sequense varibles
      long StartFan = constStartMillis + (CountDownTime / 2); // creates StartFan variable
      long StartSeperator = constStartMillis + ((CountDownTime / 2) + 5000);  // creates StartSeperator variable
      long StartCO2 = constStartMillis + (CountDownTime - 1000);  // creates StartCO2 variable
      long Startfluid = constStartMillis + (CountDownTime - 1000);  // creates Startfluid variable

    // End sequense Varibles
      long EndFan = EndTimeMillis ; // creates EndFan variable
      long EndSeperator = EndTimeMillis - (EndingSequense / 2); // creates EndSeperator variable
      long EndCO2 = ExperimentTimeMillis ;  // creates EndCO2 variable
      long Endfluid = ExperimentTimeMillis ;  // creates Endfluid variable
    //

    //=============Countdown Sequense==============================

      if ((currentMillis - constStartMillis) <= 100) {    // Starts led1
        digital_outputs.set(led1, HIGH);
        Serial.println("Turned on led 1");
        delay(ownDelay);
      }

      if (currentMillis == StartFan) {    // starts External Fan
        Serial.print("Start Fan: ");
        Serial.print(StartFan);
        
        digital_outputs.set(DexternFan, HIGH);
        analog_out.write(AnExternFan, AnExternFanVoltage);
        Serial.println("\nStart Fan at "+String(AnExternFanVoltage)+"V");
        delay(ownDelay);
      }

      if (currentMillis == StartSeperator) {  // starts seperator
        Serial.print("Start Seperator: ");
        Serial.print(StartSeperator);

        digital_outputs.set(seperator, HIGH);
        analog_out.write(AnSeperator, AnSeperatorVoltage);
        Serial.println("\nStart Seperator");
        delay(ownDelay);
      }

      if (currentMillis == StartCO2) {    // Starts CO2, Fluid pump, led2 
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

      if (currentMillis == EndOfCountdown) {  // prints when the count donw time has ended and started experiment time
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

      if (currentMillis == EndCO2) {    // ends CO2, fluid pump, led2 after the experimentTime has ended 
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

      if (currentMillis == EndSeperator) {  // ends seperator
        Serial.print("Stopping separator: ");
        Serial.print(EndSeperator);      
        
        digital_outputs.set(seperator, LOW);
        analog_out.write(AnSeperator, 0);
        Serial.println("\nStoped Separator");
        delay(ownDelay);
      }

      if (currentMillis == EndFan) {  // ends external Fan, led1
        Serial.print("Stopping Fan: ");
        Serial.print(EndFan);      
        
        digital_outputs.set(DexternFan, LOW);
        analog_out.write(AnExternFan, 0);
        digital_outputs.set(led1, LOW);
        Serial.println("\nStoped Extern Fan & Turn off led1 ");  
        delay(ownDelay);
      }

      if (currentMillis == EndTimeMillis) { // prints endtime in millis when end time == current millis
        Serial.print("End Time: ");
        Serial.println(EndTimeMillis);
        delay(ownDelay);      
      }

      if (currentMillis == (EndTimeMillis + 5000)) {  // after endtime + 5 seconds end the while loop
        Serial.println("\nDone with experiment\n\n");
        int machineON = 0;
        break;
      }

    }    
  }
  else {  // do nothing
  }
}

void loop() {
  // fixa knapp function för att starta machineRun()
	check_status();
  machineRun();
}