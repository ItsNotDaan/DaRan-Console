/*
  Name: DaRan Controller
  Date:
  Author: Daan Heetkamp

  Description:
  This is the controller of the DaRan Console.
  Each controller will fucntion the same way only they will have different adresses.

  Revision:
  4.0

*/

// ----- include libraries -----
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define knop 6

// ----- Declare Constants -----
uint8_t adresHub[] = {0x00, 0xCE, 0xCC, 0xCE, 0xCC}; //Het adres voor de Hub
uint8_t adresCon[] = {0x01, 0xCE, 0xCC, 0xCE, 0xCC}; //Het adres voor alle controllers

// ----- Declare Objects -----
RF24 radio(9, 8);  // CE, CSN. This is for connecting the CE and the CSN pins of the transceiver.

// ----- Declare subroutines and/or functions -----

// ----- Declare Global Variables -----
int buzz = 7; //Buzzer
int vibr = 5; //Vibration motor
bool isGedrukt = LOW; //For making sure the controller can only press once.

struct t_message
{
  uint8_t alleCons;
  uint8_t verzenderUID;
  uint8_t ontvangerUID;
  char command;
} bericht; //Var voor het bericht bericht.verzenderUID =


// Setup
void setup()
{
  pinMode(buzz, OUTPUT); //For when the controller has won.
  pinMode(vibr, OUTPUT); //For when the controller has won.
  pinMode(knop, INPUT_PULLUP); //init the button.

  Serial.begin(9600);

  radio.begin(); //Init the transceiver

  Serial.print("Radio aangesloten : ");
  Serial.println(radio.isChipConnected() ? "JA" : "NEE");

  radio.setPayloadSize(sizeof(t_message));
  radio.setPALevel(RF24_PA_LOW);

  radio.openWritingPipe(adresHub); //Schrijf altijd naar de Hub
  radio.openReadingPipe(1, adresCon); //Luister altijd naar het adres van de controllers
  radio.startListening(); //Start listening to signals.
}

// Main loop
void loop()
{

  if (radio.available()) //If something is received. This is to know which game is being started.
  {
    Serial.println("Radio.available");
    radio.read(t_message &bericht, sizeof(t_message));
    char text = bericht.command;
    radio.startListening(); //Start listening  to incoming signals.

    if (text == '1')//Is the incoming text '1'? Start game 1.
    {
      Serial.println("Game 1");

      bool end = false;
      while (end == false) //While the end is false this while will be active.
      {
        //Serial.println("Wacht op signaal");
        if (digitalRead(knop) == LOW && isGedrukt == LOW) //If the button is pressed and the button has not yes been pressed. This way there can only be pressed ones a game.
        {
          Serial.println("Knop gedrukt"); //Show that the button has been pressed. It always gets here.
          radio.stopListening(); //Start stending.

          bericht.verzenderUID = 1;
          bericht.command = '1';
          radio.write(t_message &bericht, sizeof(t_message));

          isGedrukt = HIGH; //isGedrukt has been put on high. This will allow to not constantly press the button. No cheating ;)
          radio.startListening(); //Start listening.
        }

        if (radio.available()) //If a  signal has been found.
        {
          Serial.println("Signaal binnen"); //Show that a signal has been found.
          radio.read(t_message &bericht, sizeof(t_message));
          char in = bericht.command;
          if (bericht.alleCons == 1) //gaat het bericht naar alle controllers?
          {
            if (in == '4') //If the incoming message is '4' it means the game has been ended.
            {
              Serial.println("Signaal 4 is binnen");
              digitalWrite(vibr, LOW);
              noTone(buzz);
              end = true; //make the bool end true. The code will leave the while.
            }
          }
          if (bericht.ontvangerUID == 1 && bericht.alleCons == 0) //Gaat het bericht naar een speciafieke con?
          {
            if (in == 'F') //If the incoming message is 'F' it means you have pressed to fast.
            {
              tone(buzz, 1000); //normaal laten trillen
              digitalWrite(vibr, HIGH);
              delay(1000);
              noTone(buzz);
              digitalWrite(vibr, LOW);
            }
            else if (in == 'T')//If the incoming message is 'T' it means you are the winner.
            {
              tone(buzz, 1000);
              digitalWrite(vibr, HIGH);
            }
          }
        }
      }
      isGedrukt = LOW; //The next game the button will be able to be pressed again. Its a reset.
    }

    else if (text == '2') //Start game 2 Not important now.
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

    else if (text == '3')
    {
      tone(buzz, 1500);
    }

    ///delay(1000); //For testing purposes.
    noTone(buzz); //Make sure the buzzer is out.
  }
}
