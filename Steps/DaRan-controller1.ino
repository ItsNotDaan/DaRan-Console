/*
  Name: DaRan Ontvanger
  Date:
  Author: Daan Heetkamp

  Description:
  Zorg dat er met deze code data kan worden ontvangen.

  Revision:
  0.1

*/

// ----- include libraries -----
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define knop 6

// ----- Declare Constants -----
const byte hub[6] = "00001"; //Dit is het adres van de hub.
const byte contr[6] = "00002"; //Dit is het adres van deze controller

// ----- Declare Objects -----
RF24 radio(9, 8);  // CE, CSN. Dit is nodig voor de librarie om te kijken welke pin de ontvanger is aangesloten.

// ----- Declare subroutines and/or functions -----

// ----- Declare Global Variables -----
const int buzz = 7; //const maakt een read only
bool isGedrukt = LOW;


// Setup
void setup()
{
  pinMode(buzz, OUTPUT);
  pinMode(knop, INPUT_PULLUP);

  Serial.begin(9600); //Start een seriele verbinding.

  radio.begin(); //zorg dat de radio begint met luisteren
  radio.openReadingPipe(0, contr); //adres dat ook in de constant werd aangegeven.
  radio.startListening(); //dit is een ontvanger. .startSending is een zender.
}

// Main loop
void loop()
{
 // if (digitalRead(knop) == HIGH)
 // {
 //   Serial.println("Knop");
 // }

  if (radio.available()) //als er iets binnenkomt.
  {
    char text = {0}; //maak een array genaamd tekst waar 32 chars in kunnen. de {0} is om hem leeg te hebben. Er zijn dus 32 karakters met een 0.
    radio.read(&text, sizeof(text)); //alles dat wordt ingelezen wordt opgeslagen in de char text.
    if (text == '1')//is de binnengekomen text "1"?
    {
      Serial.println("text = 1");
      //(11 - 10 = 1) < 10000 =  true
      //(12 - 10 = 2) < 10000 = true
      //.....
      //(10010 - 10 = 10000) =< 10000 = true
      //(10011 - 10 = 10001) =< 10000 = false dus doorgaan.

      unsigned long tijdTimer = 10000; //10 seconden wachten.
      unsigned long huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
      while (millis() - huidigeTijd < tijdTimer) //doe 10 seconden alles wat in de while staat.
      {
        if (millis() - huidigeTijd > 5000)
        {
            tone(buzz, 2000);
        }
        if (digitalRead(knop) == HIGH && isGedrukt == LOW) //Als de knop wordt geklikt.
        {
          radio.stopListening(); //door te stoppen met luisteren wordt het een zender
          radio.openWritingPipe(hub);
          const char in[] = "1"; //maak een array met karakters genaamd text. Stop hierin "1".
          radio.write(&in, sizeof(in)); //data die in 'in' staat wordt verstuurd.

          isGedrukt == HIGH;
        }
      }
      noTone(buzz);
    }

    else if (text == '2')
    {
      tone(buzz, 1000);
    }

    else if (text == '3')
    {
      tone(buzz, 1500);
    }
    Serial.println(text); //text wordt gescgrevcen in de seriele monitor.
    //Serial.println(buz); //text wordt gescgrevcen in de seriele monitor.
    delay(1000);
    noTone(buzz);
  }
}
