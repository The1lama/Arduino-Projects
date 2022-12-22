# Arduino-Projects
A collection of Arduino Projects that I have done 

---

## Project desc.

* ### [Read_temperature](https://github.com/The1lama/Arduino-Projects/blob/main/Read_temperature/Read_temperature.ino) 
  * reads temperature and humidity from a DHT11 sensor
  
* ### [TextDisplay](https://github.com/The1lama/Arduino-Projects/blob/main/TextDisaplay/TextDisaplay.ino)
  * Uses a Monochrome OLED based SSD1306 drivers from adafruit that is 128x32p

* ### [countDown](https://github.com/The1lama/Arduino-Projects/blob/main/countDown/countDown.ino)
  * Makes a small count down timer using delay and for loop

* ### [doAfterXmillis](https://github.com/The1lama/Arduino-Projects/blob/main/doAfterXmillis/doAfterXmillis.ino)
  * A timer using millis for faster readings

* ### [portenta Machine Controller](https://github.com/The1lama/Arduino-Projects/tree/main/portenta%20Machine%20Controller)

  * #### [MachineControllerTestmillams](https://github.com/The1lama/Arduino-Projects/blob/main/portenta%20Machine%20Controller/MachineControllerTestmillams/MachineControllerTestmillams.ino)
    * open and closes digital_outputs after a set amount of time using millis

  * #### [portentaMachineControllerBetterWebServerNewest](https://github.com/The1lama/Arduino-Projects/blob/main/portenta%20Machine%20Controller/portentaMachineControllerBetterWebServerNewest/portentaMachineControllerBetterWebServerNewest.ino)
    * ###### Have diffrent versions of the same code
    * Creates a webserver where one can change varibles for how long a sequense is going to take, how much analog voltage it is going to use 
    * has a sequense timer from varible of how long it should run
    * has a custom button that waches the analog in and if it exedes it code main code runs

  * #### [portentaMachineControllerV1](https://github.com/The1lama/Arduino-Projects/blob/main/portenta%20Machine%20Controller/portentaMachineControllerV1/portentaMachineControllerV1.ino)
    * first draft of the code above but using delay insted of millis
  
  * #### [portentaMachineControllerWebServer](https://github.com/The1lama/Arduino-Projects/blob/main/portenta%20Machine%20Controller/portentaMachineControllerWebServer/portentaMachineControllerWebServer.ino)
    * First version of the webserver on the machine Controller

  * #### [portentaMachineControllerWmillis](https://github.com/The1lama/Arduino-Projects/blob/main/portenta%20Machine%20Controller/portentaMachineControllerWmillis/portentaMachineControllerWmillis.ino)
    * ###### Versions before
    * First test with implimenting millis based of portentaMachineControllerV1's code

  * #### [portentaMachineController_makeshit_button](https://github.com/The1lama/Arduino-Projects/blob/main/portenta%20Machine%20Controller/portentaMachineController_makeshift_button/portentaMachineController_makeshift_button.ino)
    * The test version of the makeshit button using analog input 
