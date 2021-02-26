/*
  Name: DaRan menu
  Date: 5 februari 2021
  Author: Daan Heetkamp

  Description:
  Maak een programma dat kan switchen tussen 4 standen

  Revision: 0.3

*/

// include libraries

// ----- Declare Constants -----

#define knopR 2
#define knopL 3
#define knopM 4




// ----- Declare Objects -----

// ----- Declare subroutines and/or functions -----


// ----- Declare Global Variables -----
int aantalDrukken = 1; //het aantal keer drukken staat standaard op 0 dus beginscherm.
int laatsteMenu = 1; //de plek waar het menu voor het laatste was.
int laatsteDruk = 1; // laatste case
int gedrukt = 1;
bool activeren = LOW; //hiermee laat het programma weten dat een spel moet gaan beginnen.
bool isGedrukt = LOW;

// Setup
void setup()
{
  pinMode(knopR, INPUT_PULLUP);
  pinMode(knopL, INPUT_PULLUP);
  pinMode(knopM, INPUT_PULLUP);
  Serial.begin(9600);
}

/*int drukken()
{
  if (digitalRead(knopR) == HIGH) {gedrukt = 1;}
  if (digitalRead(knopL) == HIGH) {gedrukt = 2;}
  if (digitalRead(knopM) == HIGH) {gedrukt = 3;}

  return
}*/

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
  if (aantalDrukken != laatsteMenu || activeren == HIGH)
  {
     switch(aantalDrukken)
     {
      case 1:
      Serial.print("Menu 1");
      Serial.print("\n");
      if(activeren == HIGH)
      {
       Serial.print("1 actief");
       Serial.print("\n");
       activeren = LOW;
      }
      break;

      case 2:
      Serial.print("Menu 2");
      Serial.print("\n");
      if(activeren == HIGH)
      {
       Serial.print("2 actief");
       Serial.print("\n");
       activeren = LOW;
      }
      break;

      case 3:
      Serial.print("Menu 3");
      Serial.print("\n");
      if(activeren == HIGH)
      {
       Serial.print("3 actief");
       Serial.print("\n");
       activeren = LOW;
      }
      break;

      case 4:
      Serial.print("Menu 4");
      Serial.print("\n");
      if(activeren == HIGH)
      {
       Serial.print("4 actief");
       Serial.print("\n");
       activeren = LOW;
      }
      break;
     }
    laatsteMenu = aantalDrukken;
  }
}
