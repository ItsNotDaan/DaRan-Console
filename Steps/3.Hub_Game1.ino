/*
  Name: Hub game 1
  Date:
  Author: Daan Heetkamp

  Description:
  This is game 1 of the hub. This file has been made to easilly find a "bug".

  Revision: 1.3
*/

// ----- include libraries -----
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//voor transciever
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
// ----- Declare Constants -----
#define knopM 4 //The button to activate the game.

uint8_t adresHub[] = {0x00, 0xCE, 0xCC, 0xCE, 0xCC}; //Het adres voor de Hub
uint8_t adresCon[] = {0x01, 0xCE, 0xCC, 0xCE, 0xCC}; //Het adres voor alle controllers
// ----- Declare Objects -----

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //For the lcd
RF24 radio(9, 8);  // CE, CSN. This is for connecting the CE and the CSN pins of the transceiver.

// ----- Declare subroutines and/or functions -----

// ----- Declare Global Variables -----
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
  pinMode(knopM, INPUT_PULLUP); //pinmode for the button.
  Serial.begin(9600);

  radio.begin(); //Init the transceiver

  Serial.print("Radio aangesloten : ");
  Serial.println(radio.isChipConnected() ? "JA" : "NEE");

  radio.setPayloadSize(sizeof(t_message));
  radio.setPALevel(RF24_PA_LOW);

  radio.openWritingPipe(adresCon); //Schrijf altijd naar de controllers
  radio.openReadingPipe(1, adresHub); //Luister altijd naar de controllers
  radio.startListening(); //Start listening to signals.

  lcd.begin(16,2);
  lcd.home(); //Init the LCD
}

// Main loop
void loop()
{
 if (digitalRead(knopM) == HIGH) //If the button is pressed.
 {
    radio.stopListening();
    bericht.command = '1'; //command 1 tells the cons that game 1 starts.
    bericht.alleCons = 1;
    sendMessage(bericht);
    //radio.write(t_message &bericht, sizeof(t_message)); //Send the data in 'text'.
    bericht.alleCons = 0;
    radio.startListening();
    //The above needs to be done to let the controller know it starts game 1. It always does this. No bug.

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  CLICK BUTTON  ");

    //(11 - 10 = 1) < 10000 =  true
    //(12 - 10 = 2) < 10000 = true
    //.....
    //(10010 - 10 = 10000) =< 10000 = true
    //(10011 - 10 = 10001) =< 10000 = false so go out of the while.
    long tijdTimer = 5000; //Let the while wait for 5 seconds.
    long huidigeTijd = millis();
    while (millis() - huidigeTijd < tijdTimer) //Do the stuff in the while for 5 secs.
    {
      //Serial.println("while");
      if (radio.available()) //If a signal is available. Here lies the bug. one times it does find something and the next time not.
      {
        Serial.println("Radio is available");
        //radio.read(t_message &bericht, sizeof(t_message));
        getMessage(bericht);
        char in = bericht.verzenderUID; //Make a local char called "in"
        //Now it knows which controller has won.

        lcd.setCursor(0,0);
        lcd.print(" The winner is: ");
        lcd.setCursor(0,2);
        lcd.print("   Player:");
        lcd.setCursor(11,2);
        lcd.print(in); //Show on the LCD who has won.
        if (in == '1') //Winner con1? '1' means that controller 1 has won.
        {
          radio.stopListening(); //start writing
          radio.openWritingPipe(con1);
          bericht.command = 'T'; //command 1 tells the cons that game 1 starts //write 'T' to con1. It knows it has won and will execute his code.
          bericht.ontvangerUID = 1;
          sendMessage(bericht);
          //radio.write(t_message &bericht, sizeof(t_message));
          radio.startListening();
        }
        delay(4000); //Show for 4 seconds who has won.
        tijdTimer = 0; //make tijdTimer 0. Now it will go out of the while.
      }
    }
    radio.stopListening(); //Start writing.

    bericht.command = '4'; //Put "4" in this char. //This will be send to all the controllers. They know they can go back to the beginning.
    bericht.alleCons = 1;
    sendMessage(bericht);
    //radio.write(t_message &bericht, sizeof(t_message)); //Send the data in 'text'.
    bericht.alleCons = 0;
    radio.startListening();
    lcd.clear();
  }
}

void sendMessage(t_message &msg)
{
  radio.write(&msg, sizeof(t_message))
}

bool getMessage(t_message &msg)
{
  radio.read(&msg, sizeof(t_message));
}
