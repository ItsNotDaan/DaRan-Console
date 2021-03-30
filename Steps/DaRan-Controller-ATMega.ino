/*
  Name: DaRan Ontvanger
  Date:
  Author: Daan Heetkamp

  Description:
  Zorg dat er met deze code data kan worden ontvangen.

  Revision:
  3.0

*/

// ----- include libraries -----
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define knop 6

// ----- Declare Constants -----
const byte hub[6] = "00001"; //Dit is het adres van de hub.
const byte con2[6] = "00003"; //Dit is het adres van deze controller

// ----- Declare Objects -----
RF24 radio(9, 8);  // CE, CSN. Dit is nodig voor de librarie om te kijken welke pin de ontvanger is aangesloten.

// ----- Declare subroutines and/or functions -----

// ----- Declare Global Variables -----
int buzz = 7; //Buzzer
int vibr = 5; //Vibration motor
bool isGedrukt = LOW;
unsigned char adr;

// Setup
void setup()
{
  pinMode(buzz, OUTPUT);
  pinMode(vibr, OUTPUT);
  pinMode(knop, INPUT_PULLUP);

  radio.begin(); //zorg dat de radio begint met luisteren
  radio.openReadingPipe(1, con2); //adres dat ook in de constant werd aangegeven. Lezen
  //radio.openWritingPipe(con1); //adres dat ook in de constant werd aangegeven.Schrijven
  radio.openWritingPipe(hub);
  radio.startListening(); //dit is een ontvanger. .startSending is een zender.
}

// Main loop
void loop()
{
  if (radio.available()) //als er iets binnenkomt.
  {
    char text[] = {0}; //maak een array met karakters genaamd text. Stop hierin "1".
    radio.read(&text, sizeof(text)); //alles dat wordt ingelezen wordt opgeslagen in de char text.

    if (text[0] == '1')//is de binnengekomen text '1'? Spel 1 start.
    {
      radio.startListening();
      bool end = false;

      //Dit kan dus niet, hij moet wachten hiero. Anders gaat die verder
      while (end == false) //Geen signaal binnen? blijf wachten
      {
        //Serial.println("Wacht op signaal");

        if (digitalRead(knop) == HIGH && isGedrukt == LOW) //Als de knop wordt geklikt.
        {
          radio.stopListening(); //door te stoppen met luisteren wordt het een zender
          const char in[] = "2"; //maak een array met karakters genaamd in. Stop hierin "1".
          radio.write(&in, sizeof(in)); //data die in 'in' staat wordt verstuurd.
          radio.startListening();
          isGedrukt = HIGH;
        }
        if (radio.available()) //Als tekst 4 binnenkomt.
        {
          char in[] = {0};
          radio.read(&in, sizeof(in)); //alles dat wordt ingelezen wordt opgeslagen in de char text
          if (in[0] == '4') //Einde van game 4.
          {
            digitalWrite(vibr, LOW);
            noTone(buzz);
            end = true;
          }
          else if (in[0] == 'F') //Te snel gedrukt?
          {
            tone(buzz, 1000); //normaal laten trillen
            digitalWrite(vibr, HIGH);
            delay(1000);
            noTone(buzz);
            digitalWrite(vibr, LOW);
          }
          else if (in[0] == 'T')//Winnaar?
          {
            tone(buzz, 1000);
            digitalWrite(vibr, HIGH);
          }

        }
      }
      isGedrukt = LOW;
    }

    else if (text == 2)
    {
      //Serial.println("text = 1");
      //(11 - 10 = 1) < 10000 =  true
      //(12 - 10 = 2) < 10000 = true
      //.....
      //(10010 - 10 = 10000) =< 10000 = true
      //(10011 - 10 = 10001) =< 10000 = false dus doorgaan.

      unsigned long tijdTimer = 10000; //10 seconden wachten.
      unsigned long huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
      while (millis() - huidigeTijd < tijdTimer) //doe 10 seconden alles wat in de while staat.
      {
        if (millis() - huidigeTijd > 8000)
        {
            tone(buzz, 2000);
        }
        if (digitalRead(knop) == HIGH && isGedrukt == LOW) //Als de knop wordt geklikt.
        {
          radio.stopListening(); //door te stoppen met luisteren wordt het een zender
          const char in[] = "1"; //maak een array met karakters genaamd in. Stop hierin "1".
          radio.write(&in, sizeof(in)); //data die in 'in' staat wordt verstuurd.
          radio.startListening();
          isGedrukt = HIGH;
        }
      }
      isGedrukt = LOW;
      noTone(buzz);
    }

    else if (text == 3)
    {
      tone(buzz, 1500);
    }
    //Serial.println(text); //text wordt gescgrevcen in de seriele monitor.
    //Serial.println(buz); //text wordt gescgrevcen in de seriele monitor.
    delay(1000);
    noTone(buzz);
  }
}
