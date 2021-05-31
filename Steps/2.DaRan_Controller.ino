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
int led = 5; //Vibration motor
bool isGedrukt = false; //For making sure the controller can only press once.
byte controllerNr = 2;

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
  pinMode(led, OUTPUT); //For when the controller has won.
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
    //radio.read(t_message &bericht, sizeof(t_message));
    leesBericht(bericht);
    char text = bericht.command;
    radio.flush_rx();
    radio.flush_tx();


    /***********************************************************************************************/
    if (text == '1')//Is the incoming text '1'? Start game 1.
    {
      Serial.println("Game 1");
      //digitalWrite(led, HIGH);

      bool end = false;
      isGedrukt = false;
      while (end == false) //While the end is false this while will be active.
      {
        //Serial.println("Wacht op signaal");
        if (digitalRead(knop) == LOW && isGedrukt == false) //If the button is pressed and the button has not yes been pressed. This way there can only be pressed ones a game.
        {
          tone(buzz, 1000, 100);
          Serial.println("Knop gedrukt"); //Show that the button has been pressed. It always gets here.

          bericht.verzenderUID = controllerNr;
          stuurBericht(bericht);

          //radio.write(t_message &bericht, sizeof(t_message));

          isGedrukt = true; //isGedrukt has been put on high. This will allow to not constantly press the button. No cheating ;)
        }

        if (radio.available()) //If a  signal has been found.
        {
          Serial.println("Signaal binnen"); //Show that a signal has been found.
          //radio.read(t_message &bericht, sizeof(t_message));
          leesBericht(bericht);
          char in = bericht.command;
          Serial.println(in);
          if (bericht.alleCons == 1 && in == '4') //gaat het bericht naar alle controllers?
          {
            //if (in == '4') //If the incoming message is '4' it means the game has been ended.
            //{
              if (bericht.ontvangerUID == controllerNr) //Controller gewonnen?
              {
                digitalWrite(led, HIGH);
                gewonnen(); //duurt 2 seconden
                delay(2000);
              }
              else {
                delay(4000); //4 seconden wachten
              }
              Serial.println("Signaal 4 is binnen");
              digitalWrite(led, LOW);
              end = true; //make the bool end true. The code will leave the while.
            //}
          }
          if (bericht.alleCons == 0 && in == 'F') //Gaat het bericht naar een speciafieke con?
          {
            //if (in == 'F') //If the incoming message is 'F' it means you have pressed to fast.
            //{
              if (bericht.ontvangerUID == controllerNr)
              {
                tone(buzz, 300, 500);
                digitalWrite(led, HIGH);
                delay(1000);
              }
            //}
          }
        }
      }
      //while(digitalRead(knop) == LOW);
      isGedrukt = false; //The next game the button will be able to be pressed again. Its a reset.
    }


    /***********************************************************************************************/
    else if (text == '2') //Start game 2 Not important now.
    {
      Serial.println("Game 2 starts");
      //(11 - 10 = 1) < 10000 =  true
      //(12 - 10 = 2) < 10000 = true
      //.....
      //(10010 - 10 = 10000) =< 10000 = true
      //(10011 - 10 = 10001) =< 10000 = false dus doorgaan.
      isGedrukt = false;
      long tijdTimer = 10000; //10 seconden wachten.
      unsigned long huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
      while (millis() - huidigeTijd < tijdTimer) //doe 10 seconden alles wat in de while staat.
      {
        if (digitalRead(knop) == LOW && isGedrukt == false) //Als de knop wordt geklikt.
        {
          tone(buzz,1000,100);
          Serial.println("Knop gedrukt");
          bericht.verzenderUID = controllerNr;
          Serial.println(bericht.verzenderUID);
          stuurBericht(bericht);
          isGedrukt = true;
        }
      }
      //digitalWrite(led, LOW);
      isGedrukt = false;
      bool gedrukt = false;
      bool eind = false;
      while (eind == false) //Doe je mee?
      {
        if (radio.available()) //Is er een signaal binnen?
        {
          Serial.println("Data komt binnen");
          leesBericht(bericht);
          Serial.println(bericht.ontvangerUID);
          Serial.println(bericht.alleCons);

          if (bericht.ontvangerUID == controllerNr && bericht.alleCons == 0) //Is het signaal voor deze controller?
          {
            Serial.println("Controller moet gooien");
            digitalWrite(led, HIGH); //Led zodat controller weet dat die mag.
            long tijdTimer = 5000; //5 seconden wachten.
            unsigned long huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
            while (millis() - huidigeTijd < tijdTimer) //Na 5 seconden klikt die automatisch.
            {
              if (digitalRead(knop) == LOW && isGedrukt == false) //Knop kan gedrukt worden.
              {
                tone(buzz, 1000, 100);
                Serial.println("Controller gooit");
                bericht.verzenderUID = controllerNr;
                Serial.println(bericht.verzenderUID);
                stuurBericht(bericht);
                isGedrukt = true;
                gedrukt = true;
                tijdTimer = 0;
              }
            }
            if (gedrukt == false) //Knop nog niet gedrukt na 5 seconden?
            {
              Serial.println("Controller heeft niet gedrukt in 5 seconden");
              bericht.verzenderUID = controllerNr;
              stuurBericht(bericht);
              gedrukt = true;
            }
            digitalWrite(led, LOW); //ledje uit want controller hoeft niet meer te gooien.
          }

          if (bericht.alleCons == 1 && bericht.command == '4') //Einde van het spel.
          {
            Serial.println("Signaal 4 is binnen");

            if (bericht.ontvangerUID == controllerNr) //Controller gewonnen?
            {
              digitalWrite(led, HIGH);
              gewonnen(); //duurt 2 seconden
              delay(2000);
            }
            else {
              delay(4000); //4 seconden wachten
            }
            digitalWrite(led, LOW);
            eind = true; //Stop het spel en ga terug naar het begin.
          }
        }
      }
    }



    /***********************************************************************************************/
    else if (text == '3')
    {
      Serial.println("Game 3 starts");

      isGedrukt = false;
      long tijdTimer = 10000; //10 seconden wachten.
      unsigned long huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
      while (millis() - huidigeTijd < tijdTimer) //doe 10 seconden alles wat in de while staat.
      {
        if (digitalRead(knop) == LOW && isGedrukt == false) //Als de knop wordt geklikt.
        {
          Serial.println("Controller toegevoegd");
          bericht.verzenderUID = controllerNr;
          stuurBericht(bericht);
          isGedrukt = true;
        }
      }
      isGedrukt = false;


      bool klik = false;
      bool eind = false;
      while (eind == false) //Doe je mee?
      {
        if (radio.available()) //Is er een signaal binnen?
        {
          leesBericht(bericht);
          Serial.println("INFO BINNEN");
          isGedrukt = false;

          if (bericht.alleCons == 0 && bericht.ontvangerUID == controllerNr)
          {
            Serial.println("AAN DE BEURT");

            tijdTimer = 5000; //5 seconden wachten.
            huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
            while (millis() - huidigeTijd < tijdTimer)
            {
              if (digitalRead(knop) == LOW && isGedrukt == false) //Als de knop wordt geklikt.
              {
                bericht.verzenderUID = controllerNr;
                stuurBericht(bericht);
                isGedrukt = true;
                Serial.println("GEGOOID");
                tijdTimer = 0;
              }
            }
            if (isGedrukt == false) //Na 5 seconden nog niet gedrukt? verstuur.
            {
              bericht.verzenderUID = controllerNr;
              stuurBericht(bericht);
            }
          }

          if (bericht.alleCons == 1 && bericht.command == '4') //Einde van het spel.
          {
            Serial.println("EINDE GAME");
            if (bericht.ontvangerUID == controllerNr) //Controller verloren?
            {
              digitalWrite(led, HIGH);
              verloren(); //duurt 2 seconden
              delay(2000);
            }
            else {
              delay(4000); //4 seconden wachten
            }
            digitalWrite(led, LOW);
            eind = true; //Stop het spel en ga terug naar het begin.
          }
        }
      }

    ///delay(1000); //For testing purposes.
    noTone(buzz); //Make sure the buzzer is out.
    }
  }
}

void gewonnen(){//Duurt 2 seconden
  int a = 500;
  for(int i = 0; i < 5; i++)
  {
    tone(buzz,a,150);
    delay(150);
    a = a + 100;
  }
  tone(buzz,1000,300);
  delay(400);
  tone(buzz,1000,300);
  delay(400);
  tone(buzz,1000,600);
  delay(600);
}

void verloren(){ //Duurt 2 seconden
  int a = 500;
  for(int i = 0; i < 5; i++)
  {
    tone(buzz,a,150);
    delay(150);
    a = a + 100;
  }
  tone(buzz,600,300);
  delay(400);
  tone(buzz,400,300);
  delay(400);
  tone(buzz,200,600);
  delay(600);
}
void stuurBericht(t_message &msg)
{
  radio.stopListening();
  radio.write(&msg, sizeof(t_message));
  radio.startListening();
}

bool leesBericht(t_message &msg)
{
  radio.read(&msg, sizeof(t_message));
}
