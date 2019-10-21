///////////////////////////////////////////////////////////////// Transimtter's Code ////////////////////////////////////////////////////////////////////////
#include <SoftwareSerial.h>
SoftwareSerial mySerial(7, 8);
const int xInput = A0;
const int yInput = A1;
const int zInput = A2;
const double DegreeCon = 572957795 / 10000000;
const float xError = 1, yError = 1, zError = 1;
const int cali = 498;
// initialize minimum and maximum Raw Ranges for each axis
int RawMin = 0;
int RawMax = 1023;

// Take multiple samples to reduce noise
const int sampleSize = 10;

void setup()
{
  analogReference(EXTERNAL);
  Serial.begin(4800);
  Serial.flush();
  mySerial.begin(4800);
  mySerial.flush();
}

void loop()
{
  //Read raw values
  int xRaw = ReadAxis(xInput);
  // Convert raw values to 'milli-Gs"
  long xScaled = map(xRaw, RawMin, RawMax, -3000, 3000);
  //  long zScaled = map(zRaw, RawMin, RawMax, -3000, 3000);
  // re-scale to fractional Gs
  float xAccel = xScaled / 1000.0;

  double rad ;

  Serial.print("X :: ");
  Serial.println(xRaw-cali);
  // Scaling the analog data to Degree by mapping
  rad = map(xRaw - cali, 0 , 100 , 0,90); 
  Serial.print(rad);

  // Tranmitter
  int msg = (int)rad;
  mySerial.print(msg);

  delay(500);
}


// Take samples and return the average
int ReadAxis(int axisPin)
{
  long reading = 0;
  analogRead(axisPin);
  delay(1);
  for (int i = 0; i < sampleSize; i++)
  {
    reading += analogRead(axisPin);
  }
  return reading / sampleSize;
}




///////////////////////////////////////////////////////////////// Receiver Code ////////////////////////////////////////////////////////////////////////
/*
#include "LedControl.h"
#include "FontLEDClock.h"
#include <SoftwareSerial.h>
SoftwareSerial mySerial(7, 8);

LedControl lc = LedControl(5, 6, 4, 4 );

void setup() {

  Serial.begin(4800);
  short devices = lc.getDeviceCount ();
  mySerial.begin(4800);
  for (short address = 0 ; address < devices; address ++) {
    lc.shutdown(address, false);
    lc.setIntensity(address, 8);
    lc.clearDisplay(address);
  }


}
int j = 2;
char T[50];
void loop() {
 
  mySerial.listen();
  String text = "" ;
  ////////////////// GET Data Untill The End of It //////////////////
  while (mySerial.available())
  {
    text += (char)mySerial.read();
    delay(1);
  }
  ////////////////// Displaying Data to Matrix  //////////////////
  int i = 0;
  if (text != "") {
    clear_display();
    while (text[i]) {
      Serial.print(text[i]);
      if (text[i] >= '0' && text[i] <= '9' || text[i] == '-') {
        if (text[i] == '-') {
          for (int i = 2; i < 6 ; i++)
            lc.setLed(3, 4 , i , true);
        }
        else
          print_normal_char((i * 8), 1, text[i], 0);
      }
      i++;

    }
    print_normal_char((i * 8), 1, 'C', 0);
    delay(200);

  }
}

void plot (uint8_t x, uint8_t y, uint8_t value, bool mode) {
  //select which matrix depending on the x coord
  uint8_t address;
  if (x >= 0 && x <= 7)   {
    address = 3;
  }
  if (x >= 8 && x <= 15)  {
    address = 2;
    x = x - 8;
  }
  if (x >= 16 && x <= 23) {
    address = 1;
    x = x - 16;
  }
  if (x >= 24 && x <= 31) {
    address = 0;
    x = x - 24;
  }
  if (mode) {
    if (value == 1) {
      lc.setLed(address, 7 - x, y, true);
    } else {
      lc.setLed(address, 7 - x, y, false);
    }
  } else if (!mode) {
    if (value == 1) {
      lc.setLed(address, y, x, true);
    } else {
      lc.setLed(address, y, x, false);
    }
  }
}

void print_normal_char(uint8_t x, uint8_t y, char c, bool mode) {
  uint8_t dots;
  if (c >= 'A' && c <= 'Z' ) {
    c &= 0x1F;  // A-Z maps to 1-26
  }
  else if (c >= 'a' && c <= 'z') {
    c = (c - 'a') + 41;  // A-Z maps to 41-67
  }
  else if (c >= '0' && c <= '9') {
    c = (c - '0') + 31;
  }
  else if (c == ' ') {
    c = 0;  // space
  }
  else if (c == '.') {
    c = 27;  // full stop
  }
  else if (c == '\'') {
    c = 28;  // single quote mark
  }
  else if (c == ':') {
    c = 29;  // clock_mode selector arrow
  }
  else if (c == '>') {
    c = 30;  // clock_mode selector arrow
  }
  else if (c >= -80 && c <= -67) {
    c *= -1;
  }

  for (uint8_t col = 0; col < 5; col++) {
    dots = pgm_read_byte_near(&myfont[c][col]);
    for (uint8_t row = 0; row < 7; row++) {
      if (dots & (64 >> row)) {   // only 7 rows.
        plot(x + col, y + row, 1, mode);
      } else {
        plot(x + col, y + row, 0, mode);
      }
    }
  }
}

void clear_display() {
  for (uint8_t address = 0; address < 4; address++) {
    lc.clearDisplay(address);
  }
}
*/
