# STM32-BlackPill-FanPumpController
## Introduction
 This Project is about building a controller for PWM-fans and pumps with a display and mounting-bracket. It utilizes a STM32-BlackPill-Board (stm32f103c8), which is programmed with STM32duino.
 
 <----Fotos of finished System
 
## 1. Parts-Selection
The controller needs to do 3 different tasks:
 - Controll at least 2 PWM-fans/pumps
 - Read the temperature from a watercooling temperature-sensor
 - Display the information on a little display
 
The chosen Microcontroller is an stm32f103c8, because it is very inexpensive and has more than enough performance for this application. It has 5 5V-tolerant (important) PWM-Pins and a 16 bit ADC (analog to digital converter).

As a display the SSD1306 was chosen for very similar reasons and its fitting size.

The temperature-sensor and watercooling-pump in this case is a Barrow TCWD-V1 and SPB17-TM, but any PWM-pump and temperature-sensor (thermistor type) with a datasheet should work.

## 2. Planning and Breadboard-Mockup
The PWM-signal that will be generated has to have a 5V amplitude and a 25 kHz frequency. The information in the signal is transferred through the Duty-Cycle of the signal. 
The general specifications and tolance windows are published by noctua in the following white-paper: 
https://noctua.at/pub/media/wysiwyg/Noctua_PWM_specifications_white_paper.pdf

Because this microcontroller does not have a high enough logic level to generate this signal by itself (only 3.3V), we have to use a pullup-resistor connected to a 5V line. Then we can run the pins in open-drain mode, which gives us the required 5V PWM-signal.

From this detailed pinout-diagram (and/or the datasheet) we can see which pins we can use and how much current they can sink:
https://github.com/Jedzia/LabDocuments/blob/master/ARM/Black_Pill_Pinout-adv.png

8 mA is the recommendet current, so if we use a 1 kOhm resisitor we have a 5 mA pullup-current, which is close enough.

The display is controlled by the I2C-Bus, so we just need to connect it to power and an SCL and SDA Pin and give the scetch the correct adress. The Adress can be obtained either from the datasheet or by using this scetch: https://learn.adafruit.com/scanning-i2c-addresses/arduino

### Breadboard-Mockup PWM

<img src="Images/Mockup_PWM_1.jpg" width="300" height="400">   <img src="Images/Mockup_PWM_2.jpg" width="300" height="400">
