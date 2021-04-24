/*
  Name:
  Date:
  Author: Daan Heetkamp

  Description:

  Revision:

*/

// ----- include libraries -----
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//voor transciever
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
// ----- Declare Constants -----
#define knopM 4

const byte Rcon1[6] = "10000"; //Controller 1 dat wordt gelezen.
const byte con1[6] = "10001"; //Adres van controller 1 voor het verzenden.
// ----- Declare Objects -----

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //voor lcd
RF24 radio(9, 8);  // CE, CSN Dit is nodig voor de library om te kijken welke pinnen de zender is aangesloten.

// ----- Declare subroutines and/or functions -----

// ----- Declare Global Variables -----

void(* resetFunc) (void) = 0;

// Setup
void setup()
{
  pinMode(knopM, INPUT_PULLUP);
  Serial.begin(9600);
  radio.begin(); //start de zender

  radio.openReadingPipe(1, Rcon1); //adres dat ook in de constant werd aangegeven. Lezen
  radio.startListening();
  lcd.begin(16,2);
  lcd.home();
}

// Main loop
void loop()
{
 if (digitalRead(knopM) == HIGH)
 {
    char text = '1'; //maak een array met karakters genaamd text. Stop hierin "1".
    radio.stopListening(); //door te stoppen met luisteren wordt het een zender.
    radio.openWritingPipe(con1);
    radio.write(&text, sizeof(text)); //verstuur de data in de text.
    radio.startListening();

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  CLICK BUTTON  ");
    long tijdTimer = 5000; //for testing
    long huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
    while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
    {
      Serial.println("while");
      if (radio.available()) //als er iets binnenkomt.
      {
        Serial.println("Radio is avaiable");
        char in;
        radio.read(&in, sizeof(in)); //alles dat wordt ingelezen wordt opgeslagen in de char in.

        lcd.setCursor(0,0);
        lcd.print(" The winner is: ");
        lcd.setCursor(0,2);
        lcd.print("   Player:");
        lcd.setCursor(11,2);
        lcd.print(in);
        char win = 'T'; //maak een array met karakters genaamd text. Stop hierin "T" van tone.
        if (in == '1') //Winnaar con1?
        {
          radio.stopListening(); //door te stoppen met luisteren wordt het een zender.
          radio.openWritingPipe(con1);
          radio.write(&win, sizeof(win)); //verstuur de data in de text.
          radio.startListening();
        }
        delay(4000);
        tijdTimer = 0; //Stop de timer
      }
    }
    radio.stopListening(); //door te stoppen met luisteren wordt het een zender.
    char eind = '4'; //maak een array met karakters genaamd text. Stop hierin "4".
    radio.openWritingPipe(con1);
    radio.write(&eind, sizeof(eind)); //verstuur de data in de text.
    radio.startListening();
    lcd.clear();
    //resetFunc();
    //lcd.setCursor(0,0);
    //lcd.print(" Geen RESET ");
  }
}
