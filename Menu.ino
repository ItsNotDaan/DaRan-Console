/*
  Name: DaRan Software
  Date: 5 februari 2021
  Author: Daan Heetkamp

  Description:
  Maak een programma dat drie spelletjes kan laten spelen. Doormiddel van de console.

  Revision: 0.1
*/

// include libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define BACKLIGHT_PIN     14

// ----- Declare Constants -----

#define knopR 1
#define knopL 2
#define knopM 3



// ----- Declare Objects -----
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// ----- Declare subroutines and/or functions -----


// ----- Declare Global Variables -----
int aantalDrukken = 0; //het aantal keer drukken staat standaard op 0 dus beginscherm.
int laatstePlek = 0; //de plek waar het menu voor het laatste was.
bool activeren = LOW; //hiermee laat het programma weten dat een spel moet gaan beginnen.

// Setup
void setup()
{
  pinMode(knopR, INPUT);
  pinMode(knopL, INPUT);
  pinMode(knopM, INPUT);
  Serial.begin(9600);

  lcd.begin(16,2);
  lcd.home();
  lcd.setCursor(0,0);
}

//MAIN
void main()
{
  delay(100);
  int gedruktR = digitalRead(knopR);
  int gedruktL = digitalRead(knopR);
  int gedruktM = digitalRead(knopR);

  if (gedruktR == HIGH)
  {
    //menu moet 1 naar rechts in de case
    if (aantalDrukken == 4)
    {
      aantalDrukken = 1; //als er naar rechts gegaan moet en hij staat bij het laatste menu moet er weer naar het eerste menu gaan.
    }
    else{
      aantalDrukken++
    }
  }

  if (gedruktL == HIGH)
  {
    //menu moet 1 naar links in de case
    if (aantalDrukken == 1)
    {
      aantalDrukken = 4; //als er naar links gegaan moet en hij staat bij het eerste menu moet er weer naar het laatste menu gaan.
    }
    else{
      aantalDrukken--
    }
  }

  if (gedruktM == HIGH)
  {
    activeren = 1;
  }


///////////////////////////////////////////////////////////////////////
  switch (aantalDrukken)
  {
    case 1:
    lcd.setCursor(0,0);
    lcd.print("<--Left Right-->");
    lcd.setCursor(0,2);
    lcd.print("  Press buttons  ");

    break;

    case 2:
    lcd.setCursor(0,0);
    lcd.print("Press middle to:");
    lcd.setCursor(0,2);
    lcd.print("start game 1");

    if activeren == HIGH
    {
      //spel 1

      activeren = LOW;
    }
          /*  else if(activeren == LOW){
              lcd.setCursor(4,2);
              lcd.print("Press the middle b")
              //niets
            }*/
    break;

    case 3:
    lcd.setCursor(0,0);
    lcd.print("Press middle to:");
    lcd.setCursor(0,2);
    lcd.print("start game 2");

    if activeren == HIGH
    {
      //spel 1

      activeren = LOW;
    }
          /*  else if(activeren == LOW){
              lcd.setCursor(4,2);
              lcd.print("Press the middle b")
              //niets
            }*/
    break;

    case 4:
    lcd.setCursor(0,0);
    lcd.print("Press middle to:");
    lcd.setCursor(0,2);
    lcd.print("start game 3");

    if activeren == HIGH
    {
      //spel 1

      activeren = LOW;
    }
          /*  else if(activeren == LOW){
              lcd.setCursor(4,2);
              lcd.print("Press the middle b")
              //niets
            }*/
    break;

  }
}
