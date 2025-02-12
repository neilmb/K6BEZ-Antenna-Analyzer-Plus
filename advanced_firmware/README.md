![Ham Radio 360 Logo](http://www.360workbench.com/wpimages/wp9fedbb3f_06.png "Ham Radio 360 Logo") ![Workbench Podcast Logo](http://www.360workbench.com/wpimages/wp02796e2d_06.png "Workbench Logo")

# Standard Firmware

This directory contains the standard firmware for the K6BEZ Antenna Analyzer.

Currently the firmware supports revision 6 and 7 PCBs with Arduino Pro Mini
compatible microcontrollers and Analog Devices AD9850 synthesizers.

# Installing

The firmware can be built and installed using the Arduino Integrated Development Environment (IDE).

1. Install the [Arduino IDE Editor & Compiler](https://www.arduino.cc/en/Main/Software)
2. Add the SparkFun Board Manager URLs
  * Go to File / Preferences, and paste this URL into the 'Additional Boards Manager URLs' input field...
  * https://raw.githubusercontent.com/sparkfun/Arduino_Boards/master/IDE_Board_Manager/package_sparkfun_index.json
3. Install the Arduino Pro Micro board definitions file
  * This board file will work for the SparkFun or DealExtreme Arduino Pro Micro board
  * In the Arduino IDE, click Tools / Board / then scroll to the top and click <Board Manager>
  * At the top of the Board Manager enter "sparkfun" and hit return
  * You will see multiple Sparkfun board definition sets
  * Click on the box labeled “SparkFun AVR Boards”
  * Click **Install**
4. Open the firmware project
  * Go to File / Open and select the `aa_firmware.ino` file
5. Select the target board
  * Go to Tools / Board and scroll down to select "SparkFun Pro Micro"
  * Go to Tools / Processor and select "ATmega32u4 (5V, 16 MHz)"
  * Go to Tools / Port and select the correct Serial Port. This varies based on your Operating System and computer hardware.
5. Upload the firmware
  * Go to Sketch / Upload

# Troubleshooting

The Arduino IDE and the USB connection can be inconsistent sometimes. There
are many resources online talking about programming the Arduino on every
imaginable operating system and with almost any error.

