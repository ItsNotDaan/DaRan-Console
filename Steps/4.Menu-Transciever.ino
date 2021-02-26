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

// ----- Declare Constants -----

#define knopR 3
#define knopL 2
#define knopM 4
#define led 6




// ----- Declare Objects -----
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// ----- Declare subroutines and/or functions -----


// ----- Declare Global Variables -----
int aantalDrukken = 1; //het aantal keer drukken staat standaard op 0 dus beginscherm.
int laatsteMenu = 0; //de plek waar het menu voor het laatste was.
int laatsteDruk = 1; // laatste case
int gedrukt = 1;
bool activeren = LOW; //hiermee laat het programma weten dat een spel moet gaan beginnen.
bool isGedrukt = LOW;
bool inf = true;
int temp;
String light;
int LDR = A0;
int PT100 = A1;

// Setup
void setup()
{
  pinMode(knopR, INPUT_PULLUP);
  pinMode(knopL, INPUT_PULLUP);
  pinMode(knopM, INPUT_PULLUP);
  pinMode(LDR, INPUT);
  pinMode(PT100, INPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);

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
       aantalDrukken = 1;
       lcd.clear();
       activeren = LOW;
      }
      break;

      case 2:
      Serial.print("Menu 2");
      Serial.print("\n");

      lcd.setCursor(0,0);
      lcd.print("Press middle to:");
      lcd.setCursor(0,2);
      lcd.print("  start game 1  ");

      if(activeren == HIGH)
      {
       Serial.print("2 actief");
       Serial.print("\n");
       lcd.setCursor(0,2);
       lcd.print("  game 1 actief ");
       delay(1000);
       aantalDrukken = 1;
       lcd.clear();
       activeren = LOW;
      }
      break;

      case 3:
      Serial.print("Menu 3");
      Serial.print("\n");

      lcd.setCursor(0,0);
      lcd.print("Press middle to:");
      lcd.setCursor(0,2);
      lcd.print("  start game 2  ");

      if(activeren == HIGH)
      {
       Serial.print("3 actief");
       Serial.print("\n");
       lcd.setCursor(0,2);
       lcd.print("  game 2 actief ");
       delay(1000);
       aantalDrukken = 1;
       lcd.clear();
       activeren = LOW;
      }
      break;

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
       delay(1000);
       aantalDrukken = 1;
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
  // 0-5V komt binnen. 0= 0 graden en 5 = 100 graden. 5V is 1023.
  //om de waardes terug te brengen naar voltage moet er een berekening gedaan worden.
  float a = temp * (100 / 1023.0); //nu wordt de waarde van 1023 veranderd naar 0-100C
  temp = round(a);
  return temp;
}

String lght()
{
  int a = analogRead(LDR);
  //Serial.print(a);
  //Serial.print("\n");
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
