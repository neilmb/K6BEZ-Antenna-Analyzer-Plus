// This file is part of the K6BEZ Antenna Analyzer project.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

// include the library code:
#include <LiquidCrystal.h>
#include <Yabl.h>

// * LCD RS pin to digital pin 8
// * LCD Enable pin to digital pin 9
// * LCD D4 pin to digital pin 15
// * LCD D5 pin to digital pin 14
// * LCD D6 pin to digital pin 16
// * LCD D7 pin to digital pin 10

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 15, 14, 16, 10);

#define FQ_UD 4
#define SDAT 3
#define SCLK 5

#define RESET 2

#define MODE A2
#define BAND A3

Button mode_button;
Button band_button;


long Fstart = 1000000;  // Start Frequency for sweep
long Fstop = 30000000;  // Stop Frequency for sweep
unsigned long current_freq; // Temp variable used during sweep
long serial_input_number; // Used to build number from serial stream
long num_steps = 1001; // Number of steps to use in the sweep
char incoming_char; // Character read from serial stream
byte mode_pressed = 0;
int mode = 1;

// Function prototypes??
void Perform_sweep();

void setup() {
  mode_button.attach(MODE, INPUT);
  band_button.attach(BAND, INPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Configure DDS control pins for digital output
  pinMode(FQ_UD, OUTPUT);
  pinMode(SCLK, OUTPUT);
  pinMode(SDAT, OUTPUT);
  pinMode(RESET, OUTPUT);

  // Set up analog inputs on A0 and A1, internal reference voltage
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  analogReference(DEFAULT);

  // Reset the DDS
  digitalWrite(RESET, HIGH);
  digitalWrite(RESET, LOW);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("30Mc");
}  // end setup

void loop() {
  if (mode > 0) {
    Perform_sweep();
  }

  if ((digitalRead(BAND) == LOW) or (mode_pressed == 1)) {
    mode_pressed = 0;
    mode += 1;
    if (mode == 12) {
      mode = 1;
    }
    num_steps = 1000;

    switch(mode){
      case 1:
        // Full sweep 1-30 MHz
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("30Mc");
        Fstart = 1000000;
        Fstop = 30000000;
        break;
      case 2:
        // 160m
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("160m");
        Fstart = 1500000;
        Fstop = 2300000;
        break;
      case 3:
        // 80m
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("80m");
        Fstart = 2000000;
        Fstop = 5000000;
        break;
      case 4:
        // 60m
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("60m");
        Fstart = 5000000;
        Fstop = 6000000;
        break;
      case 5:
        // 40m
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("40m");
        Fstart = 6000000;
        Fstop = 8000000;
        break;
      case 6:
        // 30m
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("30m");
        Fstart = 9000000;
        Fstop = 11000000;
        break;
      case 7:
        // 20m
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("20m");
        Fstart = 13000000;
        Fstop = 16000000;
        break;
      case 8:
        // 17m
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("17m");
        Fstart = 17000000;
        Fstop = 19000000;
        break;
      case 9:
        // 15m
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("15m");
        Fstart = 20000000;
        Fstop = 23000000;
        break;
      case 10:
        // 12m
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("12m");
        Fstart = 24000000;
        Fstop = 26000000;
        break;
      case 11:
        // 10m
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("10m");
        Fstart = 28000000;
        Fstop = 30000000;
        break;

    }
  }
}

void Perform_sweep() {
  int FWD = 0;
  int REV = 0;
  int REV_nosig = 0;
  int FWD_nosig = 0;
  double VSWR;
  double minVSWR;
  long minFreq;
  long Fstep = (Fstop - Fstart) / num_steps;
  minVSWR = 999;
  minFreq = Fstart;

  // Show a period in the upper right when a sweep starts
  lcd.setCursor(15, 0);
  lcd.write(".");

  // Reset the DDS
  digitalWrite(RESET, HIGH);
  delay(1);
  digitalWrite(RESET, LOW);
  delay(10);
  SetDDSFreq(Fstart);
  delay(100);


  // Frequency loop
  for (long i = 0; i <= num_steps; i++) {
    // Calculate current frequency
    current_freq = Fstart + i * Fstep;

    // Set DDS to current frequency
    SetDDSFreq(current_freq);
    // Wait a little for settling
    delay(10);
    // Read the forawrd and reverse voltages
    REV = analogRead(A0);
    FWD = analogRead(A1);

    if (REV >= FWD) {
      // To avoid a divide by zero or negative VSWR then set to max 999
      VSWR = 999;
    } else {
      // Calculate VSWR
      VSWR = ((double)FWD + (double)REV) / ((double)FWD - (double)REV);
    }

    if (VSWR <= minVSWR) {
      minFreq = current_freq;
      minVSWR = VSWR;
    }
  }  // end frequency loop


  // clear the period we put in the upper right
  lcd.setCursor(15, 0);
  lcd.write(" ");

  // results on line 2
  lcd.setCursor(0, 1);
  lcd.print(minFreq);
  lcd.print(",");
  lcd.print(minVSWR);
  lcd.print(":1    ");

  digitalWrite(13, HIGH);
  //delay(10);
  digitalWrite(13, LOW);
}

void SetDDSFreq(long Freq_Hz) {
  // Calculate the DDS word - from AD9850 Datasheet
  int32_t f = Freq_Hz * 4294967295 / 125000000;

  // Send one byte at a time
  for (int b = 0; b < 4; b++, f >>= 8) {
//    SPI.transfer(f & 0xFF);
    send_byte(f & 0xFF);
  }

  // 5th byte needs to be zeros
  //SPI.transfer(0);
  send_byte(0);

  // Strobe the Update pin to tell DDS to use values
  digitalWrite(FQ_UD, HIGH);
  digitalWrite(FQ_UD, LOW);
}

void send_byte(byte data_to_send) {
  // Bit bang the byte over the SPI bus
  for (int i = 0; i < 8; i++, data_to_send >>= 1) {
    // Set Data bit on output pin
    digitalWrite(SDAT, data_to_send & 0x01);
    // Strobe the clock pin
    digitalWrite(SCLK, HIGH);
    digitalWrite(SCLK, LOW);
  }
}
