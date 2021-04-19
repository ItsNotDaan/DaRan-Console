/*
  Name: DaRan Controller 2
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
#define knop 11

// ----- Declare Constants -----
const byte con2[6] = "20000"; //Dit is het verzend adres van deze controller.
const byte Rcon2[6] = "20001"; //Dit is het ontvang adres van deze controller.
// ----- Declare Objects -----
RF24 radio(9, 10);  // CE, CSN. Dit is nodig voor de librarie om te kijken welke pin de ontvanger is aangesloten.

// ----- Declare subroutines and/or functions -----

// ----- Declare Global Variables -----
int buzz = 13; //const maakt een read only
int vibr = 5;
bool isGedrukt = LOW;
unsigned char adr;

// Setup
void setup()
{
  pinMode(buzz, OUTPUT);
  pinMode(vibr, OUTPUT);
  pinMode(knop, INPUT_PULLUP);

  Serial.begin(9600); //Start een seriele verbinding.

  radio.begin(); //zorg dat de radio begint met luisteren
  radio.setPALevel(RF24_PA_LOW);     // Dicht bij elkaar? dan kan low.

  radio.openReadingPipe(1, Rcon2); //adres dat ook in de constant werd aangegeven. Lezen
  radio.openWritingPipe(con2);

  radio.startListening(); //dit is een ontvanger. .startSending is een zender.
  flush_tx();
}

// Main loop
void loop()
{
 // if (digitalRead(knop) == HIGH)
 // {
   Serial.println("Loop");
 // }
  if (radio.available()) //als er iets binnenkomt.
  {
    //Serial.println(adr);
    Serial.println("Radio.available");
    char text[] = {0}; //maak een array met karakters genaamd text. Stop hierin "1".
    radio.read(&text, sizeof(text)); //alles dat wordt ingelezen wordt opgeslagen in de char text.
    Serial.println(text);

    if (text[0] == '1')//is de binnengekomen text '1'? Spel 1 start.
    {
      //radio.startListening();
      Serial.println("Game 1");
      bool end = false;
      while (end == false) //Geen signaal binnen? blijf wachten
      {
        //Serial.println("Wacht op signaal");
        if (digitalRead(knop) == HIGH && isGedrukt == LOW) //Als de knop wordt geklikt.
        {
          Serial.println("Knop gedrukt");
          radio.stopListening(); //door te stoppen met luisteren wordt het een zender
          const char out = '2'; //maak een array met karakters genaamd in. Stop hierin "1".
          radio.write(&out, sizeof(out)); //data die in 'in' staat wordt verstuurd.
          radio.startListening();
          isGedrukt = HIGH;
        }

        if (radio.available()) //Als tekst 4 binnenkomt.
        {
          char in[] = {0};
          radio.read(&in, sizeof(in)); //alles dat wordt ingelezen wordt opgeslagen in de char text.
          Serial.println("Signaal binnen");
          Serial.println(text);
          if (in[0] == '4')
          {
            Serial.println("Signaal 4 is binnen");
            noTone(buzz);
            digitalWrite(vibr, LOW);
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
      Serial.println("Game 2");
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
            tone(buzz, 1000);
        }
        if (digitalRead(knop) == HIGH && isGedrukt == LOW) //Als de knop wordt geklikt.
        {
          radio.stopListening(); //door te stoppen met luisteren wordt het een zender
          const char in[] = '1'; //maak een array met karakters genaamd in. Stop hierin "1".
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
  }
}
