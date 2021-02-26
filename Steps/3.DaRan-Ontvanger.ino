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

// ----- Declare Constants -----
const byte address[6] = "00001"; //Dit is het adress waar naar gekeken wordt.

// ----- Declare Objects -----
RF24 radio(9, 8);  // CE, CSN. Dit is nodig voor de librarie om te kijken welke pin de ontvanger is aangesloten.

// ----- Declare subroutines and/or functions -----

// ----- Declare Global Variables -----


// Setup
void setup()
{
  Serial.begin(9600); //Start een seriele verbinding.

  radio.begin(); //zorg dat de radio begint met luisteren

  //set the address
  radio.openReadingPipe(0, address); //adres dat ook in de constant werd aangegeven.

  //Set module as receiver
  radio.startListening(); //dit is een ontvanger. .startSending is een zender.
}

// Main loop
void loop()
{
  //Read the data if available in buffer
  if (radio.available()) //als er iets binnenkomt.
  {
    char text[32] = {0}; //maak een array genaamd tekst waar 32 chars in kunnen. de {0} is om hem leeg te hebben. Er zijn dus 32 karakters met een 0.
    radio.read(&text, sizeof(text)); //alles dat wordt ingelezen wordt opgeslagen in de char text.
    Serial.println(text); //text wordt gescgrevcen in de seriele monitor.
  }
}
