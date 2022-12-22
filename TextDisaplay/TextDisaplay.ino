
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

long randNumber;
long randNumber1; 

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino MKR WIFI 1010:       11(SDA), A12(SCL)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);
  display.display();

  delay(1000);

  // My own code that displays text 
  own_code_loop(); 
//  own_code(); 
}

void loop() {
}

void own_code() {
  display.clearDisplay();
    
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);

  display.println(F(""));

  display.println(F("Filter: "));
  display.display();      // Show initial text
    
  delay(100);

  display.print(F("Airflow: ")); // 2 print on the same line as the one under 
  display.println(F("variabel"));

  display.display();      // Show initial text

  delay(1000);
}

void own_code_loop() {
  while (true){
    int randNumber = random(1,10);
    int randNumber1 = random(20,50);
    display.clearDisplay();
    
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 0);

    display.println(F(""));
    display.display();      // Show initial text

    display.print(F("Filter: "));
    display.print(randNumber); // can't use F('...') for variabels 
    display.println(" unit");

    display.print(F("Airflow: "));
    display.print(randNumber1);
    display.println(" m/s");
    
    display.display();      // Show initial text

    delay(2000);
  }
}
