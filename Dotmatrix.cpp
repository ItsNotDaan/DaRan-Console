#include "Dotmatrix.h"

#define Num_Of_Word 4


const unsigned char One[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0xFF, /*"1",0*/
  0xFF, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x0F, 0xFF, /*"2",0*/
}
const unsigned char Two[Num_Of_Word][32] =
{
  0xFF, 0xF8, 0xF3, 0xF7, 0xF7, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFD, 0xFB, 0xF3, 0xF0, 0xFF, /*"1",0*/
  0xFF, 0x1F, 0xCF, 0xEF, 0xEF, 0xEF, 0xEF, 0xDF, 0xBF, 0x7F, 0xFF, 0xFF, 0xFF, 0xEF, 0x0F, 0xFF, /*"2",0*/
}

const unsigned char Three[Num_Of_Word][32] =
{
  0xFF, 0xF8, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0xF8, 0xFF,
  0xFF, 0x1F, 0xEF, 0xEF, 0xEF, 0xEF, 0xEf, 0x1F, 0x1F, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0x1F, 0xFF,
}

const unsigned char DiceOne[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xFC, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x3F, 0x3F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
}

const unsigned char DiceTwo[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF,
}

const unsigned char DiceThree[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xFC, 0xFC, 0xFC, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x3F, 0x3F, 0x3F, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF,
}

const unsigned char DiceFour[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF, 0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF,
  0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF, 0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF,
}

const unsigned char DiceFive[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xFC, 0xFC, 0xFC, 0xFC, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF,
  0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0x3F, 0x3F, 0x3F, 0x3F, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF,
}

const unsigned char DiceSix[Num_Of_Word][32] =
{
  0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF,
  0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF,
}