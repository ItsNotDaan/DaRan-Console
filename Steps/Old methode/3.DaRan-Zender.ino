/*
  Name: DaRan Zender
  Date:
  Author: Daan Heetkamp

  Description:
  Met deze code kan er verzonden worden met de zender.

  Revision:
  0.1

*/

// ----- include libraries -----
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// ----- Declare Constants -----
const byte address[6] = "00001"; //Dit is het adress waar naar verzonden wordt.

// ----- Declare Objects -----
RF24 radio(9, 8);  // CE, CSN Dit is nodig voor de library om te kijken welke pinnen de zender is aangesloten.

// ----- Declare subroutines and/or functions -----

// ----- Declare Global Variables -----


// Setup
void setup()
{
  radio.begin(); //start de zender

  radio.openWritingPipe(address); //init het adres van de zender.

  radio.stopListening(); //door te stoppen met luisteren wordt het een zender.
}

// Main loop
void loop()
{
  //Send message to receiver
  const char text[] = "Hello World"; //maak een array met karakters genaamd text. Stop hierin "Hello World".
  radio.write(&text, sizeof(text)); //verstuur de data in de text.

  delay(1000); //wachtb 1 seconden met het verzenden van het bericht.
}
