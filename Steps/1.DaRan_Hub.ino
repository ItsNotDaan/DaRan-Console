/*
  Name: DaRan Software
  Date: 26 februari 2021
  Author: Daan Heetkamp

  Description:
  Maak een programma dat drie spelletjes kan laten spelen. Doormiddel van de console.

  Revision: 4.0

*/

// include libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//voor transciever
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Arduino.h>

// ----- Declare Constants -----

#define knopR 3
#define knopL 2
#define knopM 4
#define led 6

//SCHERM INITS**********************************************************************************

#define LEDARRAY_D 37
#define LEDARRAY_C 35
#define LEDARRAY_B 33
#define LEDARRAY_A 31
#define LEDARRAY_G 29
#define LEDARRAY_DI 27
#define LEDARRAY_CLK 25
#define LEDARRAY_LAT 23

#define Num_Of_Word 4

const unsigned char Nr1[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0xFF, /*"1",0*/
  0xFF, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x0F, 0xFF, /*"2",0*/
};
const unsigned char Nr2[Num_Of_Word][32] =
{
  0xFF, 0xF8, 0xF3, 0xF7, 0xF7, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFD, 0xFB, 0xF3, 0xF0, 0xFF, /*"1",0*/
  0xFF, 0x1F, 0xCF, 0xEF, 0xEF, 0xEF, 0xEF, 0xDF, 0xBF, 0x7F, 0xFF, 0xFF, 0xFF, 0xEF, 0x0F, 0xFF, /*"2",0*/
};
const unsigned char Nr3[Num_Of_Word][32] =
{
  0xFF, 0xF8, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0xF8, 0xFF,
  0xFF, 0x1F, 0xEF, 0xEF, 0xEF, 0xEF, 0xEf, 0x1F, 0x1F, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0x1F, 0xFF,
};
const unsigned char Nr4[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF0, 0xFF, 0xFF, 0xFF, 0xF7, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0x0F, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xFF, 0xFF,
};
const unsigned char Nr5[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xF0, 0xF7, 0xF7, 0xF7, 0xF7, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF,
  0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0x0F, 0xFF, 0xFF,
};
const unsigned char Nr6[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xF0, 0xF7, 0xF7, 0xF7, 0xF7, 0xF0, 0xF7, 0xF7 ,0xF7, 0xF7, 0xF0, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xEF, 0xEF, 0xEF, 0xEF, 0x0F, 0xFF, 0xFF, 0xFF,
};
const unsigned char Nr7[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xF0, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0x0F, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};
const unsigned char Nr8[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xF0, 0xF7, 0xF7, 0xF7, 0xF7, 0xF0, 0xF7, 0xF7, 0xF7, 0xF7, 0xF0, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0x0F, 0xEF, 0xEF, 0xEF, 0xEF, 0x0F, 0xEF, 0xEF, 0xEF, 0xEF, 0x0F, 0xFF, 0xFF, 0xFF,
};
const unsigned char Nr9[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xF0, 0xF7, 0xF7, 0xF7, 0xF7, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0x0F, 0xEF, 0xEF, 0xEF, 0xEF, 0x0F, 0xEF, 0xEF, 0xEF, 0xEF, 0x0F, 0xFF, 0xFF, 0xFF ,
};
const unsigned char Nr10[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xF4, 0xE5, 0xF5, 0xF5, 0xF5, 0xF5, 0xF5, 0xF5, 0xF5, 0xF5, 0xF5, 0xE0, 0xFF, 0xFF,
  0xFF, 0xFF, 0x07, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0x07, 0xFF, 0xFF,
};
const unsigned char Nr11[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xFD, 0xF1, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xF0, 0xFF, 0xFF,
  0xFF, 0xFF, 0xEF, 0x8F, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0x87, 0xFF, 0xFF,
};
const unsigned char Nr12[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xF4, 0xC7, 0xF7, 0xF7, 0xF7, 0xF4, 0xF5, 0xF5, 0xF5, 0xF5, 0xF5, 0xE0 ,0xFF, 0xFF,
  0xFF, 0xFF, 0x0F, 0x7F, 0x7F, 0x7F, 0x7F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x0F, 0xFF, 0xFF,
};
const unsigned char Nr13[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xF7, 0xC4, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xE0, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x0F, 0x7F, 0x7F, 0x7F, 0x7F, 0x0F, 0x7F, 0x7F, 0x7F, 0x7F, 0x0F, 0xFF, 0xFF,
};
const unsigned char Nr14[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xF7, 0xC5, 0xF5, 0xF5, 0xF5, 0xF5, 0xF4, 0xF7, 0xF7, 0xF7, 0xF7, 0xE3, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0x0F, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xFF, 0xFF,
};
const unsigned char Nr15[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xF7, 0xC4, 0xF5, 0xF5, 0xF5, 0xF5, 0xF4, 0xF7, 0xF7, 0xF7, 0xF7, 0xE0, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x7F, 0x7F, 0x7F, 0x7F, 0x0F, 0xFF, 0xFF,
};
const unsigned char Nr16[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xF7, 0xC4, 0xF5, 0xF5, 0xF5, 0xF5, 0xF4, 0xF5, 0xF5, 0xF5, 0xF5, 0xE0, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x7F, 0x7F, 0x7F, 0x7F, 0x0F, 0xFF, 0xFF,
};
const unsigned char Nr17[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xF7, 0xC4, 0xF5, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xE3, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x0F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0xFF, 0xFF,
};
const unsigned char Nr18[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xF7, 0xC4, 0xF5, 0xF5, 0xF5, 0xF5, 0xF4, 0xF5, 0xF5, 0xF5, 0xF5, 0xE0, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x0F, 0x7F, 0x7F, 0x7F, 0x7F, 0x0F, 0x7F, 0x7F, 0x7F, 0x7F, 0x0F, 0xFF, 0xFF,
};
const unsigned char Nr19[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xF7, 0xC4, 0xF5, 0xF5, 0xF5, 0xF5, 0xF4, 0xF7, 0xF7, 0xF7, 0xF7, 0xE0, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x0F, 0x7F, 0x7F, 0x7F, 0x7F, 0x0F, 0x7F, 0x7F, 0x7F, 0x7F, 0x0F, 0xFF, 0xFF,
};
const unsigned char Nr20[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xFF, 0xE0, 0xFE, 0xFE, 0xFE, 0xFE, 0xE0, 0xEF, 0xEF, 0xEF, 0xEF, 0xE0, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x83, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0x83, 0xFF, 0xFF,
};
const unsigned char An1[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC7, 0xC7, 0xA7, 0xDF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};
const unsigned char An2[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFD, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x3F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};
const unsigned char An3[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x7F, 0x6F, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};
const unsigned char An4[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x2F, 0x07, 0x7F, 0x6F, 0x17, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};
const unsigned char An5[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0x57, 0x2F, 0x07, 0x7B, 0x6F, 0x17, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};
const unsigned char An6[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFD, 0xFE, 0xFF, 0xFC, 0xFB, 0xFE, 0xFE, 0xFD, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xD7, 0x57, 0x2F, 0x07, 0x7B, 0x6D, 0x17, 0xEB, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF,
};
const unsigned char An7[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xFD, 0xFE, 0xFF, 0xF9, 0xFE, 0xFF, 0xFC, 0xFB, 0xF6, 0xFE, 0xFD, 0xFD, 0xFF, 0xFF,
  0xFF, 0xFF, 0xD7, 0xD7, 0x57, 0x2F, 0x07, 0x7B, 0x6D, 0x16, 0xEB, 0xEB, 0xEF, 0xFF, 0xFF, 0xFF,
};
const unsigned char An8[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xF9, 0xFE, 0xFF, 0xF1, 0xFE, 0xFF, 0xFC, 0xFB, 0xF6, 0xFE, 0xFD, 0xFD, 0xFD, 0xFF,
  0xFF, 0xD7, 0xD7, 0xD7, 0x57, 0x2F, 0x07, 0x7B, 0x6D, 0x16, 0xEB, 0xEB, 0xEB, 0xEF, 0xFF, 0xFF,
};
const unsigned char An9[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xF1, 0xFE, 0xFF, 0xE1, 0xFE, 0xFF, 0xFC, 0xFB, 0xF6, 0xFE, 0xFD, 0xFD, 0xFD, 0xFD,
  0xD7, 0xD7, 0xD7, 0xD7, 0x57, 0x2F, 0x07, 0x7B, 0x6D, 0x16, 0xEB, 0xEB, 0xEB, 0xEF, 0xFF, 0xFF,
};
const unsigned char An10[Num_Of_Word][32] =
{
  0xFF, 0xFF, 0xF1, 0xFE, 0xFF, 0xC1, 0xFE, 0xFF, 0xFC, 0xFB, 0xF6, 0xFE, 0xFD, 0xFD, 0xFD, 0xFD,
  0xD7, 0xD7, 0xD7, 0xD7, 0x57, 0x2F, 0x07, 0x7B, 0x6D, 0x16, 0xEB, 0xEB, 0xEB, 0xEF, 0xFF, 0xFF,
};
const unsigned char Vol[Num_Of_Word][32] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


unsigned char Display_Buffer[8];
unsigned char Display_Swap_Buffer[1][32] = {0};

/************************************************************************************************************************8*/

//Dit is het adressen waar naar verzonden/ontvangen wordt.
uint8_t adresHub[] = {0x00, 0xCE, 0xCC, 0xCE, 0xCC}; //Het adres voor de Hub
uint8_t adresCon[] = {0x01, 0xCE, 0xCC, 0xCE, 0xCC}; //Het adres voor alle controllers

// ----- Declare Objects -----
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //voor lcd
RF24 radio(9, 8);  // CE, CSN Dit is nodig voor de library om te kijken welke pinnen de zender is aangesloten.

// ----- Declare subroutines and/or functions -----


// ----- Declare Global Variables -----
int aantalDrukken = 1; //het aantal keer drukken staat standaard op 0 dus beginscherm.
int laatsteMenu = 0; //de plek waar het menu voor het laatste was.
int laatsteDruk = 1; // laatste case
int gedrukt = 1;
bool activeren = LOW; //hiermee laat het programma weten dat een spel moet gaan beginnen.
bool isGedrukt = LOW;
int temp;
unsigned long laatsteTijd = 0; //Laatste tijd. Is nodig om timers te maken.
String light;
int LDR = A0;
int PT100 = A1;

struct t_message
{
  uint8_t alleCons;
  uint8_t verzenderUID;
  uint8_t ontvangerUID;
  char command;
} bericht; //Var voor het bericht bericht.verzenderUID =

// Setup
void setup()
{
  pinMode(knopR, INPUT_PULLUP);
  pinMode(knopL, INPUT_PULLUP);
  pinMode(knopM, INPUT_PULLUP);
  pinMode(LDR, INPUT);
  pinMode(PT100, INPUT);
  pinMode(led, OUTPUT);

  pinMode(LEDARRAY_D, OUTPUT);
  pinMode(LEDARRAY_C, OUTPUT);
  pinMode(LEDARRAY_B, OUTPUT);
  pinMode(LEDARRAY_A, OUTPUT);
  pinMode(LEDARRAY_G, OUTPUT);
  pinMode(LEDARRAY_DI, OUTPUT);
  pinMode(LEDARRAY_CLK, OUTPUT);
  pinMode(LEDARRAY_LAT, OUTPUT);

  Clear_Display();

  Serial.begin(9600);

  radio.begin(); //Init the transceiver

  Serial.print("Radio aangesloten : ");
  Serial.println(radio.isChipConnected() ? "JA" : "NEE");

  radio.setPayloadSize(sizeof(t_message));
  radio.setPALevel(RF24_PA_LOW);

  radio.openWritingPipe(adresCon); //Schrijf altijd naar de controllers
  radio.openReadingPipe(1, adresHub); //Luister altijd naar de controllers
  radio.startListening(); //Start listening to signals.

  radio.startListening();

  lcd.begin(16,2);
  lcd.home();

  lcd.setCursor(0,0);
  lcd.print(" DaRan Console  ");
  lcd.setCursor(0,2);
  lcd.print("Push the buttons");
  delay(2000);
  lcd.clear();
}

//MAIN
void loop()
{
  if (digitalRead(knopR) == HIGH) {gedrukt = 1; isGedrukt = HIGH;}
  if (digitalRead(knopL) == HIGH) {gedrukt = 2; isGedrukt = HIGH;}
  if (digitalRead(knopM) == HIGH) {gedrukt = 3; isGedrukt = HIGH;}

  if(isGedrukt == HIGH)
  {
    switch (gedrukt)
    {
      case 1:
      aantalDrukken++;

      if (aantalDrukken == 5)
      {
        aantalDrukken = 1;
      }
      break;

      case 2:
      aantalDrukken--;

      if (aantalDrukken == 0)
      {
        aantalDrukken = 4;
      }
      break;

     case 3:
     activeren = HIGH;
     break;
    }
    isGedrukt = LOW;
  }
  menu();
  delay(200);
}

void menu()
{
  if (aantalDrukken != laatsteMenu || activeren == HIGH || aantalDrukken == 1)
  {
     switch(aantalDrukken)
     {
      case 1:
      lcd.setCursor(0,0);
      lcd.print(" Press a button ");
      lcd.setCursor(0,2);
      lcd.print("                ");
      lcd.setCursor(0,2);
      lcd.print("Temp: ");

      //lcd.print(" Press a button ");
      //lcd.print("Temp: ..C   Day ");
      //lcd.print("Temp: ..C  Night");

      lcd.setCursor(6,2);
      tmp();
      lcd.print(temp); //gaat heel snel. Later oplossing voorbedenken.
      if (temp < 10) {
        lcd.setCursor(8,2);
        lcd.print(" ");
        lcd.setCursor(7,2);
        lcd.print("C");
      } else {
        lcd.setCursor(8,2);
        lcd.print("C");
      }

      lght();
      if (light == "Night") {
        lcd.setCursor(11,2);
      }
      else {
        lcd.setCursor(11,2);
        lcd.print("     "); //deze twee omdat anders night nog kan blijven staan. Kan ook met een laatste maar dan moet er een nieuwe var komen.
        lcd.setCursor(12,2);
      }

      lcd.print(light);

      if(activeren == HIGH)
      {
       lcd.setCursor(0,0);
       lcd.print("To find a game: ");
       lcd.setCursor(0,2);
       lcd.print("Press right/left");
       delay(2000);
       aantalDrukken = 1; //terug naar start Menu
       lcd.clear();
       activeren = LOW;
      }
      break;

/************************************************************************************************/

      case 2: //fastest presser
      Serial.print("Menu 2");
      Serial.print("\n");

      lcd.setCursor(0,0);
      lcd.print("Press middle to:");
      lcd.setCursor(0,2);
      lcd.print("  start game 1  ");

      if(activeren == HIGH)
      {
        lcd.setCursor(0,0);
        lcd.print("Fastest Presser ");
        lcd.setCursor(0,2);
        lcd.print("     Active     ");

        bericht.command = '1'; //command 1 tells the cons that game 1 starts.
        bericht.alleCons = 1; //Alle controllers moeten dit commando aannemen.
        stuurBericht(bericht); //Verstuur het bericht.
        bericht.alleCons = 0; //Zet alle controllers uit.

        lcd.clear();
        //16x16 dot display = "3"
        lcd.setCursor(8,0);
        lcd.print("3");
        Animatie_scherm(3, 1000);

        //16x16 dot display = "2"
        lcd.setCursor(8,0);
        lcd.print("2");
        Animatie_scherm(2, 1000);

        //16x16 dot display = "1"
        lcd.setCursor(8,0);
        lcd.print("1");
        Animatie_scherm(1, 1000);

        //16x16 dot display = "Timer start"
        lcd.setCursor(0,0);
        lcd.print("  Timer starts  ");



        //(11 - 10 = 1) < 10000 =  true
        //(12 - 10 = 2) < 10000 = true
        //.....
        //(10010 - 10 = 10000) =< 10000 = true
        //(10011 - 10 = 10001) =< 10000 = false dus doorgaan.
        //unsigned long tijdTimer = random(5000, 20000); //random tijd tussen de 5 en 20 seconden
        long tijdTimer = 5000; //for testing
        unsigned long huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
        while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
        {
          if (radio.available()) //als er iets binnenkomt.
          {
            leesBericht(bericht);
            Serial.println(bericht.verzenderUID);
            Serial.print("heeft te snel gedrukt");

            bericht.command = 'F'; //F is van verloren
            bericht.ontvangerUID = bericht.verzenderUID;
            stuurBericht(bericht);
          }
        }

        lcd.setCursor(0,0);
        lcd.print("CLICK THE BUTTON");
        lcd.setCursor(0,2);
        lcd.print("      NOW!      ");

        tijdTimer = 10000; //Wacht 10 seconden. Als er niets is gedrukt dan gwn terug.
        huidigeTijd = millis(); //tijd hoelang het programma al draait.
        while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
        {
          Display(Vol);
          //animatie dat timer over is
          if(radio.available())
          {
            Serial.println("Winnaar is binnen");

            leesBericht(bericht);

            lcd.setCursor(0,0);
            lcd.print(" The winner is: ");
            lcd.setCursor(0,2);
            lcd.print("   Player:");
            lcd.setCursor(11,2);
            lcd.print(bericht.verzenderUID); //Show on the LCD who has won.


            bericht.ontvangerUID = bericht.verzenderUID;

            //**************************************************
            //Het terugsturen geeft een bug. Voor nu overslaan.
            /*
            bericht.command = 'T'; //T is een win.
            bericht.ontvangerUID = bericht.verzenderUID;
            sendMessage(bericht); //Hier zit de fout
            */
            //**************************************************

            // delay(4000);
            tijdTimer = 0; //Stop de timer
          }
        }

        bericht.command = '4'; //Put "4" in this char. //This will be send to all the controllers. They know they can go back to the beginning.
        bericht.alleCons = 1;
        stuurBericht(bericht);
        bericht.alleCons = 0;
        Animatie_scherm(A1, 200);
        Animatie_scherm(A2, 200);
        Animatie_scherm(A3, 200);
        Animatie_scherm(A4, 200);
        Animatie_scherm(A5, 200);
        Animatie_scherm(A6, 200);
        Animatie_scherm(A7, 200);
        Animatie_scherm(A8, 200);
        Animatie_scherm(A9, 200);
        Animatie_scherm(A10, 200);


        aantalDrukken = 1; //terug naar start Menu
        activeren = LOW;
        lcd.clear();
      }
      break;

      /*for (int i = 0; i <= e; i++) //loop door geklikt om te kijken of deze persoon al heeft geklikt.
      {
        if (geKlikt[i] == in[0]) //Als hij nog niet is geKlikt
        {
          fout = true //Als die fout is dan fout = true
        }
      }*/
/************************************************************************************************/

      case 3: //Dobbelsteen
      Serial.print("Menu 3");
      Serial.print("\n");

      lcd.setCursor(0,0);
      lcd.print("Press middle to:");
      lcd.setCursor(0,2);
      lcd.print("  start game 2  ");

      if(activeren == HIGH)
      {
        lcd.setCursor(0,0);
        lcd.print("  Dobbelsteen   ");
        lcd.setCursor(0,2);
        lcd.print("     Active     ");

        bericht.command = '2'; //command 1 tells the cons that game 1 starts.
        stuurBericht(bericht); //Verstuur het bericht.

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  Press to add  ");
        lcd.setCursor(0,2);
        lcd.print("your controller ");

        //(11 - 10 = 1) < 10000 =  true
        //(12 - 10 = 2) < 10000 = true
        //.....
        //(10010 - 10 = 10000) =< 10000 = true
        //(10011 - 10 = 10001) =< 10000 = false dus doorgaan.
        int aangemeld[4]; //een array met 4 plekjes
        int e = 0; //Voor het bijvoegen van dingen in het array
        unsigned char adr;
        long tijdTimer = 10000; //10 seconden wachten.
        unsigned long huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
        while (millis() - huidigeTijd < tijdTimer) //doe 10 seconden lang mensen toevoegen die mee willen doen.
        {
         if (radio.available()) //als er iets binnenkomt.
         {
           Serial.print("Bericht binnen");
           leesBericht(bericht); //Lees het inkomende bericht
           aangemeld[e] = bericht.verzenderUID; //Elke keer als er iets binnenkomt dan wordt de waarde van de controller in de aangemelde array gegooid.
           e++;
           lcd.clear();
           lcd.setCursor(0,0);
           lcd.print("  Player: ");
           lcd.setCursor(11,0);
           lcd.print(bericht.verzenderUID);
           lcd.setCursor(0,2);
           lcd.print("     Added    ");

           delay(500);

           lcd.setCursor(0,0);
           lcd.print("  Press to add  ");
           lcd.setCursor(0,2);
           lcd.print("your controller ");
           //Doordat er bij de controller maar 1 keer gedrukt kan worden staat alles erin.
         }
        }

        int spelers = e;
        /*for (int i = 0; i < sizeof(aangemeld); i++) //Kan het variable van e ook gebruikt worden??
        {
          if (aangemeld[i] != 0)
          {
             spelers++;
          }
        }*/

        Serial.println(e); //testen
        Serial.println(spelers); //testen
        int rondes;
        int points[4] = {};
        char pointName[4] = {};
        while (spelers != rondes) //Rondes hetzelfde als het aantal mensen dat had gedrukt?
        {
          for (int a = 0; a < spelers; a++) //loop van 0 tot het aantal mensen dat heeft gedrukt.
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(" Controller: ");
            lcd.setCursor(14,0);
            lcd.print(aangemeld[a]);
            lcd.setCursor(0,2);
            lcd.print("   can throw    ");
            bericht.ontvangerUID = aangemeld[a]; //Geef door aan de controller dat die mag gooien

            stuurBericht(bericht); //Verstuur het bericht.

            bool klik = false;
            while (klik == false) //klik false?
            {
              if (radio.available()) //signaal binnen?
              {
                bool klaar = false;
                leesBericht(bericht);
                if (bericht.verzenderUID == aangemeld[a]) //Zelfde als degene die als eerste mocht gooien?
                {
                  int gooi = random(1,6); //maak een getal tussen de 1 en 6.
                  points[a] = gooi; //De waarde van gooi in array points.
                  pointName[a] = bericht.verzenderUID; //De controller naam van de gooier op dezelfde plek als de hoogste.

                  klik = true;
                }
              }
            }
          }
          rondes++; //Langs alle spelers gaan.
        }

        int aantal1, aantal2, aantal;
        aantal1 = max(points[0], points[1]); //Max van deze twee in aantal1;
        aantal2 = max(points[2], points[3]); //Max van deze twee in aantal2;
        aantal = max(aantal1, aantal2); //max van de 4 in aantal.
        Serial.println(aantal);
        int winner/*[4] = {}*/;
        int b;
        for (int a = 0; a < 4; a++) //ga voor 4 keer kijken welk nummer het hoogste is.
        {
          /*if (points[a] == winner) //is er nog een keer de waarde van de winnaar?
          {
            b++
          }*/

          if (points[a] == aantal) //komt de waarde overeen met het getal in het array.
          {
            winner/*[b]*/ = a; //de waarde van a in winnaar stoppen.
          }
          //winner kan nu zijn {2,4,0,0} Controller 2 en 4 zijn dus dubbel en moeten nog een keer.
        }
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("    Who has    ");
        lcd.setCursor(0,2);
        lcd.print("    Won????    ");
        delay(2000);
        /*
        De naam van de winnaar wordt uit het array gehaald.
        pointName[0,3,2,0]
        points[0,1,6,0]
        Controller 2 is de winnaar met 6 punten.
        */
        lcd.clear();
        Serial.println("Speler ");
        Serial.print(pointName[winner]);
        Serial.print("has won with: ");
        Serial.print(points[winner]);
        Serial.print("points\n");

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Controller   won");
        lcd.setCursor(12,0);
        lcd.print(pointName[winner]);

        lcd.setCursor(0,2);
        lcd.print(" With   Points ");
        lcd.setCursor(6,2);
        lcd.print(points[winner]);

        bericht.ontvangerUID = pointName[winner]; //de Winnaar wordt gestopt in ontvangerUID.
        bericht.command = '3';
        bericht.alleCons = 1; //Alle controllers moeten dit commando aannemen.
        stuurBericht(bericht); //Verstuur het bericht.
        bericht.alleCons = 0; //Zet alle controllers uit.

        delay(5000);
        aantalDrukken = 1; //terug naar start Menu
        lcd.clear();
        activeren = LOW;
       }
       break;

/************************************************************************************************/

      case 4:
      Serial.print("Menu 4");
      Serial.print("\n");

      lcd.setCursor(0,0);
      lcd.print("Press middle to:");
      lcd.setCursor(0,2);
      lcd.print("  start game 3  ");

      if(activeren == HIGH)
      {
       Serial.print("4 actief");
       Serial.print("\n");
       lcd.setCursor(0,2);
       lcd.print("  game 3 actief ");

       //radio.begin(); //start de zender
       //radio.openWritingPipe(control1); //init waar de zender naartoe moet verzenden.
       radio.stopListening(); //door te stoppen met luisteren wordt het een zender.

       const char text[] = "3"; //maak een array met karakters genaamd text. Stop hierin "Hello World".
       radio.write(&text, sizeof(text)); //verstuur de data in de text.

       delay(1000);
       aantalDrukken = 1; //terug naar start Menu
       lcd.clear();
       activeren = LOW;
      }
      break;
     }
    laatsteMenu = aantalDrukken;
  }
}



void tmp()
{
  temp = analogRead(PT100);
  //Serial.println(temp);
  // 0-5V komt binnen. 0= 0 graden en 5 = 100 graden. 5V is 1023.
  //om de waardes terug te brengen naar voltage moet er een berekening gedaan worden.
  int a = temp * (50 / 1023.0); //nu wordt de waarde van 1023 veranderd naar 0-100C
  temp = round(a);
  //temp = 19;
  return temp;
}

String lght()
{
  int a = analogRead(LDR);
  //Serial.print(a);
 // Serial.print("\n");
  if (a <= 10) //ligt eraan hoe aangesloten. Kan ook boven de 1000 zijn.
  {
    light = "Night";
  }
  else
  {
    light = "Day";
  }
  return light;
}

void stuurBericht(t_message &msg)
{
  radio.stopListening();
  radio.write(&msg, sizeof(t_message));
  radio.startListening();
}

bool leesBericht(t_message &msg)
{
  radio.read(&msg, sizeof(t_message));
}

/*************************************************16x16**************************************************************/
void Clear_Display()
{
  Serial.println("Scherm wordt gecleared");
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
}

void Animatie_scherm(int A, long tijdTimer)
{
  long huidigeTijd;

  switch(A)
  {
     case 1:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(Nr1);
     }
     break;


     case 2:
     Serial.println("CASE 2");
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(Nr2);
     }
     break;

     case 3:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(Nr3);
     }
     break;

     case 4:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(Nr4);
     }
     break;

     case 5:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(Nr5);
     }
     break;

     case 6:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(Nr6);
     }
     break;

     case 7:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(Nr7);
     }
     break;

     case 8:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(Nr8);
     }
     break;

     case 9:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(Nr9);
     }
     break;

     case 10:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(Nr10);
     }
     break;

     case 11:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(Nr11);
     }
     break;

     case 12:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(Nr12);
     }
     break;

     case 13:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(Nr13);
     }
     break;

     case 14:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(Nr14);
     }
     break;

     case 15:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(Nr15);
     }
     break;

     case 16:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(Nr16);
     }
     break;

     case 17:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(Nr17);
     }
     break;

     case 18:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(Nr18);
     }
     break;

     case 19:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(Nr19);
     }
     break;

     case 20:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(Nr20);
     }
     break;

     case 21:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(An1);
     }
     break;

     case 22:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(An2);
     }
     break;

     case 23:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(An3);
     }
     break;

     case 24:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(An4);
     }
     break;

     case 25:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(An5);
     }
     break;

     case 26:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(An6);
     }
     break;

     case 27:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(An7);
     }
     break;

     case 28:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(An8);
     }
     break;

     case 29:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(An9);
     }
     break;

     case 30:
     huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
     while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
     {
      Display(An10);
     }
     break;
  }
}
