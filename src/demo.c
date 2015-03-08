/* IMPORTANT: Much of the code here assumes
 * little endian. This absolutely will not work
 * on big endian systems.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#ifndef DEF_DIGITS
#define DEF_DIGITS

const uint8_t num[10][5] = {
  {0x07,0x05,0x05,0x05,0x07}, // 0
  {0x01,0x01,0x01,0x01,0x01}, // 1
  {0x07,0x01,0x07,0x04,0x07}, // 2
  {0x07,0x01,0x07,0x01,0x07}, // 3
  {0x05,0x05,0x07,0x01,0x01}, // 4
  {0x07,0x04,0x07,0x01,0x07}, // 5
  {0x04,0x04,0x07,0x05,0x07}, // 6
  {0x07,0x01,0x01,0x01,0x01}, // 7
  {0x07,0x05,0x07,0x05,0x07}, // 8
  {0x07,0x05,0x07,0x01,0x01} // 9
};

const uint8_t colon[5] = {
  0x00,0x01,0x00,0x01,0x00 // :
};

#endif

struct BIT_INFO {
  uint16_t s4	:1; // bit 0
  uint16_t d4	:3; // bit 1-3
  uint16_t s3	:1; // bit 4
  uint16_t d3	:3; // bit 5-7
  uint16_t s2	:1; // bit 8
  uint16_t d2	:3; // bit 9-11
  uint16_t s1	:1; // bit 12
  uint16_t d1	:3; // bit 13-15
};

union HardwareRegister {
  unsigned char   bytes[2];
  uint16_t        word;
  struct BIT_INFO bits;
};

void printBits(size_t const size, void const * const ptr)
{
  unsigned char *b = (unsigned char*) ptr;
  unsigned char byte;
  int i, j;

  for (i=size-1;i>=0;i--)
  {
    for (j=7;j>=0;j--)
    {
      byte = b[i] & (1<<j);
      byte >>= j;
      printf("%u", byte);
    }
  }
  puts("");
}

void clearScreen()
{
  const char *CLEAR_SCREEN_ANSI = "\e[1;H\e[2J";
  write(STDOUT_FILENO,CLEAR_SCREEN_ANSI,12);
}

void writeRow(int row, union HardwareRegister *col)
{
  printBits(sizeof(*col), col);
}

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

int main(int argc, const char* argv[])
{
  time_t now;
  struct tm *now_tm;
  int hour, min;

  while(1)
  {
    now = time(NULL);
    now_tm = localtime(&now);
    hour = now_tm->tm_hour;
    min = now_tm->tm_min;

    clearScreen();
    buildRows(hour, min);
    sleep(1);
  }

  return 0;
}
