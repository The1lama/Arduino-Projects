/****************************************************************************************************************************
  Async_PostServer.ino

  For Portenta_H7 (STM32H7) with Vision-Shield Ethernet or Murata WiFi

  Portenta_H7_AsyncWebServer is a library for the Portenta_H7 with Vision-Shield Ethernet or Murata WiFi

  Based on and modified from ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
  Built by Khoi Hoang https://github.com/khoih-prog/Portenta_H7_AsyncWebServer
  Licensed under GPLv3 license
 *****************************************************************************************************************************/

#if !( defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) )
	#error For Portenta_H7 only
#endif

#define _PORTENTA_H7_AWS_LOGLEVEL_     1

#define USE_WIFI_PORTENTA_H7        true

#include <WiFi.h>
#warning Using WiFi for Portenta_H7.

#include <Portenta_H7_AsyncWebServer.h>

char ssid[] = "";        // your network SSID (name)
char pass[] = "";         // your network password (use for WPA, or use as key for WEP), length must be 8+

int status = WL_IDLE_STATUS;

int CountDownTimePara = 0;        // get time in ms
int ExperimentTimePara = 0;       // get time in ms
int EndingSequensePara = 0;       // get time in ms
float AnExternFanVoltPara = 7.5;  // Maximum ouput value 10.4V
float AnSeperatorVoltPara = 7.5;  // Maximum ouput value 10.4V


AsyncWebServer    server(80);


const String postForms =
"<html>\
  <head>\
    <title>AsyncWebServer POST handling</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>CountDown Time</h1><br>\
    <form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/CountDownTime/\">\
      <input type=\"text\" name=\"CountDown_Time\" value=\"0\">\
      <input type=\"submit\" value=\"Submit\">\
    </form>\
    <h1>Experiment Time</h1><br>\
    <form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/ExperimentTime/\">\
      <input type=\"text\" name=\"Experiment_Time\" value=\"0\">\
      <input type=\"submit\" value=\"Submit\">\
    </form>\
    <h1>EndingSequense</h1><br>\
    <form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/EndingSequense/\">\
      <input type=\"number\" name=\"Ending_Sequense\" value=\"0\">\
      <input type=\"submit\" value=\"Submit\">\
    </form>\
    <h1>AnExternFan Voltage</h1><br>\
    <form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/AnExternFanVoltage/\">\
      <input type=\"text\" name=\"AnExternFan_Voltage\" value=\"7.5\">\
      <input type=\"submit\" value=\"Submit\">\
    </form>\
    <h1>AnSeperator Voltage</h1><br>\
    <form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/AnSeperatorVoltage/\">\
      <input type=\"text\" name=\"AnSeperator_Voltage\" value=\"7.5\">\
      <input type=\"submit\" value=\"Submit\">\
    </form>\
  </body>\
</html>";

void handleRoot(AsyncWebServerRequest *request)
{
	request->send(200, "text/html", postForms);
}

void handlePlain(AsyncWebServerRequest *request)
{
	if (request->method() != HTTP_POST)
	{
		request->send(405, "text/plain", "Method Not Allowed");
	}
	else
	{
		request->send(200, "text/plain", "POST body was:\n" + request->arg("plain"));
	}
}

void handleForm(AsyncWebServerRequest *request)
{
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

			// message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
			// message += request->arg(i) + "\n";
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

void handleNotFound(AsyncWebServerRequest *request)
{
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

void printWifiStatus()
{
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

void setup()
{
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

	server.on("/", HTTP_GET, [](AsyncWebServerRequest * request)
	{
		handleRoot(request);
	});

	//server.on("/CountDownTime/", handlePlain);
	server.on("/CountDownTime/", HTTP_POST, [](AsyncWebServerRequest * request)
	{
		handleForm(request);
	});

	//server.on("/ExperimentTime/", handleForm);
	server.on("/ExperimentTime/", HTTP_POST, [](AsyncWebServerRequest * request)
	{
		handleForm(request);
	});

	//server.on("/EndingSequense/", handleForm);
	server.on("/EndingSequense/", HTTP_POST, [](AsyncWebServerRequest * request)
	{
		handleForm(request);
	});

	//server.on("/AnExternFanVoltage/", handleForm);
	server.on("/AnExternFanVoltage/", HTTP_POST, [](AsyncWebServerRequest * request)
	{
		handleForm(request);
	});

	//server.on("/AnSeperatorVoltage/", handleForm);
	server.on("/AnSeperatorVoltage/", HTTP_POST, [](AsyncWebServerRequest * request)
	{
		handleForm(request);
	});

	server.onNotFound(handleNotFound);

	server.begin();

	Serial.print(F("HTTP Async_PostServer started @ IP : "));
	Serial.println(WiFi.localIP());
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
    paraPrint();    
		checkstatus_timeout = millis() + STATUS_CHECK_INTERVAL;
	}


}

void loop()
{
	check_status();
}