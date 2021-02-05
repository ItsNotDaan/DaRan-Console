/*
  Name: DaRan Software
  Date: 5 februari 2021
  Author: Daan Heetkamp

  Description:
  Maak een programma dat drie spelletjes kan laten spelen. Doormiddel van de console.

  Revision: 0.1
*/

// include libraries
#include <LiquidCrystal.h>;

// ----- Declare Constants -----


// ----- Declare Objects -----
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// ----- Declare subroutines and/or functions -----

// ----- Declare Global Variables -----



// Setup
void setup()
{
  lcd.begin(16,2);
  lcd.setCursor(3,0);
  lcd.print("Mini Gamble");
  
}

// Main loop
void loop()
{



}
