/* IMPORTANT: Much of the code here assumes
 * little endian. This absolutely will not work
 * on big endian systems.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

/*** Define the bit layouts for displaying numbers ***/
#ifndef DEF_DIGITS
#define DEF_DIGITS

const uint8_t num[10][5] = {
  {0b111,0b101,0b101,0b101,0b111}, // 0
  {0b001,0b001,0b001,0b001,0b001}, // 1
  {0b111,0b001,0b111,0b100,0b111}, // 2
  {0b111,0b001,0b111,0b001,0b111}, // 3
  {0b101,0b101,0b111,0b001,0b001}, // 4
  {0b111,0b100,0b111,0b001,0b111}, // 5
  {0b100,0b100,0b111,0b101,0b111}, // 6
  {0b111,0b001,0b001,0b001,0b001}, // 7
  {0b111,0b101,0b111,0b101,0b111}, // 8
  {0b111,0b101,0b111,0b001,0b001} // 9
};

const uint8_t colon[5] = {
  0b0,0b1,0b0,0b1,0b0 // :
};

#endif

/*** Define some custom types for handling registers ***/
struct BIT_INFO {
  uint16_t s4 :1; // bit 0
  uint16_t d4 :3; // bit 1-3
  uint16_t s3 :1; // bit 4
  uint16_t d3 :3; // bit 5-7
  uint16_t s2 :1; // bit 8
  uint16_t d2 :3; // bit 9-11
  uint16_t s1 :1; // bit 12
  uint16_t d1 :3; // bit 13-15
};

union HardwareRegister {
  unsigned char   bytes[2];
  uint16_t        word;
  struct BIT_INFO bits;
};
 
/*** Declare some global vars for arduino ***/
int dataPin = 34;
int latchPin = 35;
int clockPin = 36;

int rowPin[5] = {40,41,42,43,44};

/*** Setup arduino pin modes ***/
void setup() 
{
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  for (int i=0; i<5; i++)
  {
    pinMode(rowPin[i], OUTPUT);
    digitalWrite(rowPin[i], LOW);
  };
}
 
/*** Shift register helper function ***/
void updateShiftRegister(byte reg)
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, reg);
   digitalWrite(latchPin, HIGH);
}

/*** Write each row out to the shift registers ***/
void writeRow(int row, union HardwareRegister *col)
{

  int i;
  for(i=0; i < sizeof(rowPin) / sizeof(int); i++)
  {
    if(i == row)
    {
      digitalWrite(rowPin[i], HIGH);
    } else {
      digitalWrite(rowPin[i], LOW);
    }
  }
  
  updateShiftRegister(col->bytes[0]);
  updateShiftRegister(col->bytes[1]);
  delay(3); // Sleep for persistence of vision
}

/*** Build the registers for each row ***/
void buildRows(int hour, int min)
{
  // Separate the hour and minute digits
  uint8_t h1 = (hour / 10) % 10;
  uint8_t h2 = hour % 10;
  uint8_t m1 = (min / 10) % 10;
  uint8_t m2 = min % 10;

  union HardwareRegister reg;

  int i;
  for(i=0; i < 5; i++)
  {
    reg.word = 0;
    reg.bits.d1 = num[h1][i];
    reg.bits.d2 = num[h2][i];
    reg.bits.d3 = num[m1][i];
    reg.bits.d4 = num[m2][i];
    //reg.bits.s2 = colon[i];

    writeRow(i, &reg);
  }
}

/*** Main loop ***/
void loop()
{
  int hour, minute;
  hour = 12;
  minute = 34;

  buildRows(hour, minute);
}
