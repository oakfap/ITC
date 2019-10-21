#include "LedControl.h"
#include "FontLEDClock.h"
#include <TimeLib.h>


LedControl lc = LedControl(5, 6, 4, 4 );

int timer1_counter;
int sec = 0 , mi = 0 , Hour = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // initialize timer1
  noInterrupts(); // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  timer1_counter = 3036;
  TCNT1 = timer1_counter; TCCR1B |= (1 << CS12); TIMSK1 |= (1 << TOIE1);
  interrupts();

  int devices = lc.getDeviceCount(); // find no of LED modules
  //we have to init all devices in a loop
  for (int address = 0; address < devices; address++) { // set up each device
    lc.shutdown(address, false);
    lc.setIntensity(address, 8);
    lc.clearDisplay(address);
  }
}

ISR(TIMER1_OVF_vect) // interrupt service routine
{
  TCNT1 = timer1_counter; // preload timer
  Serial.println("Interrupt Time :" + (String)(++sec));
  if (sec >= 60) {
    mi++;
    sec = 0;
  }
  if (mi >= 60)  {
    Hour++;
    mi = 0;
  }

}

void loop() {
  
  String Time = addZero(Hour) + ":" + addZero(mi) + ":" + addZero(sec) ;
  int p = 0;
  while (Time[p]) {
    // Serial.print(Time[p]);
    print_tiny_char((p * 4), 2, Time[p]); p++;
  }
  print_tiny_char(31, 2, '.');
  Serial.println();
}

/////////////////////////////////////////////////////// Function /////////////////////////////////////////////////////////
void plot (uint8_t x, uint8_t y, uint8_t value) {

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

  if (value == 1) {
    lc.setLed(address, y, x, true);
  } else {
    lc.setLed(address, y, x, false);
  }
}

void print_tiny_char(uint8_t x, uint8_t y, char c) {
  uint8_t dots;
  if (c >= 'A' && c <= 'Z' || (c >= 'a' && c <= 'z') ) {
    c &= 0x1F;  // A-Z maps to 1-26
  }
  else if (c >= '0' && c <= '9') {
    c = (c - '0') + 32;
  }
  else if (c == ' ') {
    c = 0;   // space
  }
  else if (c == '.') {
    c = 27;  // full stop
  }
  else if (c == ':') {
    c = 28;  // colon
  }
  else if (c == '\'') {
    c = 29;  // single quote mark
  }
  else if (c == '!') {
    c = 30;  // single quote mark
  }
  else if (c == '?') {
    c = 31;  // single quote mark
  }

  for (uint8_t col = 0; col < 3; col++) {
    dots = pgm_read_byte_near(&mytinyfont[c][col]);
    for (uint8_t row = 0; row < 5; row++) {
      if (dots & (16 >> row))
        plot(x + col, y + row, 1);
      else
        plot(x + col, y + row, 0);
    }
  }
}

void clear_display() {
  for (uint8_t address = 0; address < 4; address++) {
    lc.clearDisplay(address);
  }
}

String addZero(int s) {
  String str = "";
  if (s / 10 == 0) {
    str = "0" + (String)s;
  } else str = (String)s;
  return str;
}
