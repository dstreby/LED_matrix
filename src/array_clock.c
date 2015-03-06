/* IMPORTANT: Much of the code here assumes
 * little endian. This absolutely will not work
 * on big endian systems.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

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

#endif

struct BIT_INFO {
  uint16_t b0_2   :3; // bit 0-2
  uint16_t b3     :1; // bit 3
  uint16_t b4_6   :3; // bit 4-6
  uint16_t b7     :1; // bit 7
  uint16_t b8_10  :3; // bit 8-10
  uint16_t b11    :1; // bit 11
  uint16_t b12_14 :3; // bit 12-14
  uint16_t b15    :1; // bit 15
};

union HardwareRegister {
  unsigned char   bytes[2];
  uint16_t        word;
  struct BIT_INFO bits;
};

// On actual project, this would be the call
// to update the shift registers. We're printing
// it here just for test purposes.
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

int main(int argc, const char* argv[])
{

  union HardwareRegister reg;

  int i;
  for(i=0; i < 5; i++)
  {
    reg.word = 0;
    reg.bits.b0_2 = num[4][i];
    reg.bits.b4_6 = num[3][i];
    reg.bits.b8_10 = num[2][i];
    reg.bits.b12_14 = num[1][i];
    printBits(sizeof(reg), &reg);
    //updateShiftRegister(reg.bytes[0]);
    //updateShiftRegister(reg.bytes[1]);
  }
  
  return 0;
}
