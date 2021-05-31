#include <Dotmatrix.h>
#include <SPI.h>

/***************************************************************************
                              arduino IDE v1.6.9
                              Arduino UNO R3
                              点阵屏                   Arduino UNO R3
                                A-------------------------D5
                                B-------------------------D4
                                C-------------------------D3
                                D-------------------------D2
                                G-------------------------D6
                                DI------------------------D7
                                CLK-----------------------D8
                                LAT-----------------------D9
**************************************************************************/
#include <Arduino.h>


//IO配置
#define LEDARRAY_D 2
#define LEDARRAY_C 3
#define LEDARRAY_B 4
#define LEDARRAY_A 5
#define LEDARRAY_G 6
#define LEDARRAY_DI 7
#define LEDARRAY_CLK 8
#define LEDARRAY_LAT 9
unsigned char Display_Buffer[8];
unsigned char Display_Swap_Buffer[1][32] = {0};					//显示缓冲区
#define Num_Of_Word 4

void setup()
{
  pinMode(LEDARRAY_D, OUTPUT);
  pinMode(LEDARRAY_C, OUTPUT);
  pinMode(LEDARRAY_B, OUTPUT);
  pinMode(LEDARRAY_A, OUTPUT);
  pinMode(LEDARRAY_G, OUTPUT);
  pinMode(LEDARRAY_DI, OUTPUT);
  pinMode(LEDARRAY_CLK, OUTPUT);
  pinMode(LEDARRAY_LAT, OUTPUT);

  Clear_Display();
}

void loop()
{
  long tijdTimer = 1000; //for testing
  long huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
  while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
  {
  Display(One);
  };
  huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
  while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
  {
  Display(Two);
  };
  huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
  while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
  {
  Display(Three);
  };
  huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
  while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
  {
  Display(DiceOne);
  };
  huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
  while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
  {
  Display(DiceTwo);
  };
  huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
  while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
  {
  Display(DiceThree);
  };
  huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
  while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
  {
  Display(DiceFour);
  };
  huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
  while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
  {
  Display(DiceFive);
  };
  huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
  while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
  {
  Display(DiceSix);
  };
}

void Clear_Display()
{
  unsigned char i, j;
  for (i = 0 ; i < 32 ; i++)
  {
    Display_Swap_Buffer[j][i] = 0xff;       
  }
}

void Scan_Line( unsigned char m)
{
  switch (m)
  {
    case 0:
      digitalWrite(LEDARRAY_D, LOW); digitalWrite(LEDARRAY_C, LOW); digitalWrite(LEDARRAY_B, LOW); digitalWrite(LEDARRAY_A, LOW);
      break;
    case 1:
      digitalWrite(LEDARRAY_D, LOW); digitalWrite(LEDARRAY_C, LOW); digitalWrite(LEDARRAY_B, LOW); digitalWrite(LEDARRAY_A, HIGH);
      break;
    case 2:
      digitalWrite(LEDARRAY_D, LOW); digitalWrite(LEDARRAY_C, LOW); digitalWrite(LEDARRAY_B, HIGH); digitalWrite(LEDARRAY_A, LOW);
      break;
    case 3:
      digitalWrite(LEDARRAY_D, LOW); digitalWrite(LEDARRAY_C, LOW); digitalWrite(LEDARRAY_B, HIGH); digitalWrite(LEDARRAY_A, HIGH);
      break;
    case 4:
      digitalWrite(LEDARRAY_D, LOW); digitalWrite(LEDARRAY_C, HIGH); digitalWrite(LEDARRAY_B, LOW); digitalWrite(LEDARRAY_A, LOW);
      break;
    case 5:
      digitalWrite(LEDARRAY_D, LOW); digitalWrite(LEDARRAY_C, HIGH); digitalWrite(LEDARRAY_B, LOW); digitalWrite(LEDARRAY_A, HIGH);
      break;
    case 6:
      digitalWrite(LEDARRAY_D, LOW); digitalWrite(LEDARRAY_C, HIGH); digitalWrite(LEDARRAY_B, HIGH); digitalWrite(LEDARRAY_A, LOW);
      break;
    case 7:
      digitalWrite(LEDARRAY_D, LOW); digitalWrite(LEDARRAY_C, HIGH); digitalWrite(LEDARRAY_B, HIGH); digitalWrite(LEDARRAY_A, HIGH);
      break;
    case 8:
      digitalWrite(LEDARRAY_D, HIGH); digitalWrite(LEDARRAY_C, LOW); digitalWrite(LEDARRAY_B, LOW); digitalWrite(LEDARRAY_A, LOW);
      break;
    case 9:
      digitalWrite(LEDARRAY_D, HIGH); digitalWrite(LEDARRAY_C, LOW); digitalWrite(LEDARRAY_B, LOW); digitalWrite(LEDARRAY_A, HIGH);
      break;
    case 10:
      digitalWrite(LEDARRAY_D, HIGH); digitalWrite(LEDARRAY_C, LOW); digitalWrite(LEDARRAY_B, HIGH); digitalWrite(LEDARRAY_A, LOW);
      break;
    case 11:
      digitalWrite(LEDARRAY_D, HIGH); digitalWrite(LEDARRAY_C, LOW); digitalWrite(LEDARRAY_B, HIGH); digitalWrite(LEDARRAY_A, HIGH);
      break;
    case 12:
      digitalWrite(LEDARRAY_D, HIGH); digitalWrite(LEDARRAY_C, HIGH); digitalWrite(LEDARRAY_B, LOW); digitalWrite(LEDARRAY_A, LOW);
      break;
    case 13:
      digitalWrite(LEDARRAY_D, HIGH); digitalWrite(LEDARRAY_C, HIGH); digitalWrite(LEDARRAY_B, LOW); digitalWrite(LEDARRAY_A, HIGH);
      break;
    case 14:
      digitalWrite(LEDARRAY_D, HIGH); digitalWrite(LEDARRAY_C, HIGH); digitalWrite(LEDARRAY_B, HIGH); digitalWrite(LEDARRAY_A, LOW);
      break;
    case 15:
      digitalWrite(LEDARRAY_D, HIGH); digitalWrite(LEDARRAY_C, HIGH); digitalWrite(LEDARRAY_B, HIGH); digitalWrite(LEDARRAY_A, HIGH);
      break;
    default : break;
  }
}


void Display(const unsigned char dat[][32])
{
  unsigned char i;

  for ( i = 0 ; i < 16 ; i++ )
  {
    digitalWrite(LEDARRAY_G, HIGH);   //更新数据时候关闭显示。等更新完数据，打开138显示行。防止重影。

    Display_Buffer[0] = dat[0][i];
    Display_Buffer[1] = dat[0][i + 16];

    Display_Buffer[2] = dat[1][i];
    Display_Buffer[3] = dat[1][i + 16];

    Display_Buffer[4] = dat[2][i];
    Display_Buffer[5] = dat[2][i + 16];

    Display_Buffer[6] = dat[3][i];
    Display_Buffer[7] = dat[3][i + 16];

    Send(Display_Buffer[7]);
    Send(Display_Buffer[6]);
    Send(Display_Buffer[5]);
    Send(Display_Buffer[4]);
    Send(Display_Buffer[3]);
    Send(Display_Buffer[2]);
    Send(Display_Buffer[1]);
    Send(Display_Buffer[0]);


    digitalWrite(LEDARRAY_LAT, HIGH);         //锁存数据
    delayMicroseconds(1);

    digitalWrite(LEDARRAY_LAT, LOW);
    delayMicroseconds(1);

    Scan_Line(i);           //选择第i行

    digitalWrite(LEDARRAY_G, LOW);

    delayMicroseconds(300);;    //延时一段时间，让LED亮起来。
    
  }
}

void Send( unsigned char dat)
{
  unsigned char i;
  digitalWrite(LEDARRAY_CLK, LOW);
  delayMicroseconds(1);;
  digitalWrite(LEDARRAY_LAT, LOW);
  delayMicroseconds(1);;

  for ( i = 0 ; i < 8 ; i++ )
  {
    if ( dat & 0x01 )
    {
      digitalWrite(LEDARRAY_DI, HIGH);
    }
    else
    {
      digitalWrite(LEDARRAY_DI, LOW);
    }


    digitalWrite(LEDARRAY_CLK, HIGH);        //上升沿发送数据
    delayMicroseconds(1);;
    digitalWrite(LEDARRAY_CLK, LOW);
    delayMicroseconds(1);;
    dat >>= 1;

  }
