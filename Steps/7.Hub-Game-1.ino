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

const byte Rcon1[6] = "10000"; //Controller 1 adress for recieving.
const byte con1[6] = "10001"; //Controller 1 adress for sending.
// ----- Declare Objects -----

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //For the lcd
RF24 radio(9, 8);  // CE, CSN. This is for connecting the CE and the CSN pins of the transceiver.

// ----- Declare subroutines and/or functions -----

// ----- Declare Global Variables -----

// Setup
void setup()
{
  pinMode(knopM, INPUT_PULLUP); //pinmode for the button.
  Serial.begin(9600);
  radio.begin(); //start the transciever

  radio.openReadingPipe(1, Rcon1); //Reading pipe 1 can always be read. Pipe 0 is always used for sending.
  //The openWritingPipe has been placed lower because the hub officially needs to send to multiple recievers.
  radio.startListening(); //start listening for signals.
  lcd.begin(16,2);
  lcd.home(); //Init the LCD
}

// Main loop
void loop()
{
 if (digitalRead(knopM) == HIGH) //If the button is pressed.
 {
    char text = '1'; //Make an character named text. Put "1" in this char.
    radio.stopListening();
    radio.openWritingPipe(con1); //Now it writes to con1.
    radio.write(&text, sizeof(text)); //Send the data in 'text'.
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
        char in; //Make a local char called "in".
        radio.read(&in, sizeof(in)); //The incoming char gets stored in "in".
        //Now it knows which controller has won.

        lcd.setCursor(0,0);
        lcd.print(" The winner is: ");
        lcd.setCursor(0,2);
        lcd.print("   Player:");
        lcd.setCursor(11,2);
        lcd.print(in); //Show on the LCD who has won.
        char win = 'T'; //Make an character named win. Put "T" in this char
        if (in == '1') //Winner con1? '1' means that controller 1 has won.
        {
          radio.stopListening(); //start writing
          radio.openWritingPipe(con1);
          radio.write(&win, sizeof(win)); //write 'T' to con1. It knows it has won and will execute his code.
          radio.startListening();
        }
        delay(4000); //Show for 4 seconds who has won.
        tijdTimer = 0; //make tijdTimer 0. Now it will go out of the while.
      }
    }
    radio.stopListening(); //Start writing.
    char eind = '4'; //Make an character named eind. Put "4" in this char. //This will be send to all the controllers. They know they can go back to the beginning.
    radio.openWritingPipe(con1);
    radio.write(&eind, sizeof(eind)); //verstuur de data in de text.
    radio.startListening();
    lcd.clear();
  }
}
