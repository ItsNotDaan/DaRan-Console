/*
  Name: DaRan Software
  Date: 26 februari 2021
  Author: Daan Heetkamp

  Description:
  Maak een programma dat drie spelletjes kan laten spelen. Doormiddel van de console.

  Revision: 4.0

*/

// include libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//voor transciever
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// ----- Declare Constants -----

#define knopR 3
#define knopL 2
#define knopM 4
#define led 6

//Dit is het adressen waar naar verzonden/ontvangen wordt.
//const byte address[][6] = {"00001", "00002","00003"};
const byte hub[6] = "00001"; //Dit is het adress waar naar verzonden wordt.
const byte con1[6] = "00002"; //Dit is het adress waar naar verzonden wordt.
const byte con2[6] = "00003"; //Dit is het adress waar naar verzonden wordt.
//00001 = De hub
//00002 = Controller 1
//00003 = Controller 2

// ----- Declare Objects -----
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //voor lcd
RF24 radio(9, 8);  // CE, CSN Dit is nodig voor de library om te kijken welke pinnen de zender is aangesloten.

// ----- Declare subroutines and/or functions -----


// ----- Declare Global Variables -----
int aantalDrukken = 1; //het aantal keer drukken staat standaard op 0 dus beginscherm.
int laatsteMenu = 0; //de plek waar het menu voor het laatste was.
int laatsteDruk = 1; // laatste case
int gedrukt = 1;
bool activeren = LOW; //hiermee laat het programma weten dat een spel moet gaan beginnen.
bool isGedrukt = LOW;
int temp;
unsigned long laatsteTijd = 0; //Laatste tijd. Is nodig om timers te maken.
String light;
int LDR = A0;
int PT100 = A1;

// Setup
void setup()
{
  pinMode(knopR, INPUT_PULLUP);
  pinMode(knopL, INPUT_PULLUP);
  pinMode(knopM, INPUT_PULLUP);
  pinMode(LDR, INPUT);
  pinMode(PT100, INPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  radio.begin(); //start de zender

  radio.openReadingPipe(1, con1); //adres dat ook in de constant werd aangegeven. Lezen
  //radio.openReadingPipe(2, address[2]); //adres dat ook in de constant werd aangegeven. Lezen
  radio.openWritingPipe(hub);//het apparaat dat schrijft.
  radio.startListening();
  lcd.begin(16,2);
  lcd.home();

  lcd.setCursor(0,0);
  lcd.print(" DaRan Console  ");
  lcd.setCursor(0,2);
  lcd.print("Push the buttons");
  delay(2000);
  lcd.clear();
}

//MAIN
void loop()
{
  if (digitalRead(knopR) == HIGH) {gedrukt = 1; isGedrukt = HIGH;}
  if (digitalRead(knopL) == HIGH) {gedrukt = 2; isGedrukt = HIGH;}
  if (digitalRead(knopM) == HIGH) {gedrukt = 3; isGedrukt = HIGH;}

  if(isGedrukt == HIGH)
  {
    switch (gedrukt)
    {
      case 1:
      aantalDrukken++;

      if (aantalDrukken == 5)
      {
        aantalDrukken = 1;
      }
      break;

      case 2:
      aantalDrukken--;

      if (aantalDrukken == 0)
      {
        aantalDrukken = 4;
      }
      break;

     case 3:
     activeren = HIGH;
     break;
    }
    isGedrukt = LOW;
  }
  menu();
  delay(200);
}

void menu()
{
  if (aantalDrukken != laatsteMenu || activeren == HIGH || aantalDrukken == 1)
  {
     switch(aantalDrukken)
     {
      case 1:
      lcd.setCursor(0,0);
      lcd.print(" Press a button ");
      lcd.setCursor(0,2);
      lcd.print("                ");
      lcd.setCursor(0,2);
      lcd.print("Temp: ");

      //lcd.print(" Press a button ");
      //lcd.print("Temp: ..C   Day ");
      //lcd.print("Temp: ..C  Night");

      lcd.setCursor(6,2);
      //tmp();
      lcd.print(temp); //gaat heel snel. Later oplossing voorbedenken.
      if (temp < 10) {
        lcd.setCursor(8,2);
        lcd.print(" ");
        lcd.setCursor(7,2);
        lcd.print("C");
      } else {
        lcd.setCursor(8,2);
        lcd.print("C");
      }

      lght();

      if (light == "Night") {
        lcd.setCursor(11,2);
      }
      else {
        lcd.setCursor(11,2);
        lcd.print("     "); //deze twee omdat anders night nog kan blijven staan. Kan ook met een laatste maar dan moet er een nieuwe var komen.
        lcd.setCursor(12,2);
      }

      lcd.print(light);

      if(activeren == HIGH)
      {
       lcd.setCursor(0,0);
       lcd.print("To find a game: ");
       lcd.setCursor(0,2);
       lcd.print("Press right/left");
       delay(2000);
       aantalDrukken = 1; //terug naar start Menu
       lcd.clear();
       activeren = LOW;
      }
      break;

/************************************************************************************************/

      case 2: //fastest presser
      Serial.print("Menu 2");
      Serial.print("\n");

      lcd.setCursor(0,0);
      lcd.print("Press middle to:");
      lcd.setCursor(0,2);
      lcd.print("  start game 1  ");

      if(activeren == HIGH)
      {
        lcd.setCursor(0,0);
        lcd.print("Fastest Presser ");
        lcd.setCursor(0,2);
        lcd.print("     Active     ");

        radio.stopListening(); //door te stoppen met luisteren wordt het een zender.

        char text[] = "1"; //maak een array met karakters genaamd text. Stop hierin "1".
        radio.write(&text, sizeof(text)); //verstuur de data in de text.

        radio.startListening();

        //16x16 dot display = "3"
        lcd.setCursor(8,0);
        lcd.print("3");
        delay(1000);
        //16x16 dot display = "2"
        lcd.setCursor(8,0);
        lcd.print("2");
        delay(1000);
        //16x16 dot display = "1"
        lcd.setCursor(8,0);
        lcd.print("1");
        delay(1000);
        //16x16 dot display = "Timer start"
        lcd.setCursor(0,0);
        lcd.print("  Timer starts  ");



        //(11 - 10 = 1) < 10000 =  true
        //(12 - 10 = 2) < 10000 = true
        //.....
        //(10010 - 10 = 10000) =< 10000 = true
        //(10011 - 10 = 10001) =< 10000 = false dus doorgaan.
        char geKlikt[2]; //een array met 4 plekjes
        int e = 0;
        unsigned long tijdTimer = random(5000, 20000); //random tijd tussen de 5 en 20 seconden
        unsigned long huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
        while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
        {

          if (radio.available()) //als er iets binnenkomt.
          {
            char in[] = {0};
            radio.read(&in, sizeof(in)); //alles dat wordt ingelezen wordt opgeslagen in de char in.

            int geKlikt[e] = in[0]; //Elke keer als er iets binnenkomt dan wordt de waarde van de controller in de aangemelde array gegooid.
            e++
            //Doordat er bij de controller maar 1 keer gedrukt kan worden staat alles erin.

          }
        }
        lcd.setCursor(0,0);
        lcd.print("CLICK THE BUTTON");
        lcd.setCursor(0,2);
        lcd.print("      NOW!      ");

        bool win = false;
        while(win == false)
        {
          //animatie dat timer over is
          if (radio.available())
          {
            char in[] = {0};
            radio.read(&in, sizeof(in)); //alles dat wordt ingelezen wordt opgeslagen in de char in.

            for (i = 0; i < sizeof(geKlikt[]); i++) //loop door geklikt om te kijken of deze persoon al heeft geklikt.
            {
              if (geKlikt[i] != in[0]) //Als hij nog niet is geKlikt
              {
                lcd.setCursor(0,0);
                lcd.print(" The winner is: ");
                lcd.setCursor(0,2);
                lcd.print("   Player:");
                lcd.setCursor(11,2);
                lcd.print(in[0]);

                delay(4000);
                win == true;
              }
            }
          }
        }


        aantalDrukken = 1; //terug naar start Menu
        activeren = LOW;
        lcd.clear();
      }
      break;

/************************************************************************************************/

      case 3: //Dobbelsteen
      Serial.print("Menu 3");
      Serial.print("\n");

      lcd.setCursor(0,0);
      lcd.print("Press middle to:");
      lcd.setCursor(0,2);
      lcd.print("  start game 2  ");

      if(activeren == HIGH)
      {
        lcd.setCursor(0,0);
        lcd.print("  Dobbelsteen   ");
        lcd.setCursor(0,2);
        lcd.print("     Active     ");

        radio.stopListening(); //door te stoppen met luisteren wordt het een zender.

        char text[] = "2"; //maak een array met karakters genaamd text. Stop hierin "1".
        radio.write(&text, sizeof(text)); //verstuur de data in de text.

        radio.startListening();

        //(11 - 10 = 1) < 10000 =  true
        //(12 - 10 = 2) < 10000 = true
        //.....
        //(10010 - 10 = 10000) =< 10000 = true
        //(10011 - 10 = 10001) =< 10000 = false dus doorgaan.
        char aangemeld[4]; //een array met 4 plekjes
        int e = 0;
        unsigned char adr;
        unsigned long tijdTimer = 10000; //10 seconden wachten.
        unsigned long huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
        while (millis() - huidigeTijd < tijdTimer) //doe 10 seconden alles wat in de while staat.
        {
         if (radio.available()) //als er iets binnenkomt.
         {
           char in[] = {0};
           radio.read(&in, sizeof(in)); //alles dat wordt ingelezen wordt opgeslagen in de char in.
           //Serial.println(in);
           int aangemeld[e] = in[0]; //Elke keer als er iets binnenkomt dan wordt de waarde van de controller in de aangemelde array gegooid.
           e++
           //Doordat er bij de controller maar 1 keer gedrukt kan worden staat alles erin.

           if (in[0] == '1')
           {
             Serial.println("Dit is controller 1");
           }
           else if (in[0] == '2')
           {
             Serial.println("Dit is controller 2");
           }
           else {
             Serial.println("controller niet gevonden");
           }

         }
         digitalWrite(led, HIGH);
        }

       digitalWrite(led, LOW);
       delay(1000);
       aantalDrukken = 1; //terug naar start Menu
       lcd.clear();
       activeren = LOW;
      }
      break;

/************************************************************************************************/

      case 4:
      Serial.print("Menu 4");
      Serial.print("\n");

      lcd.setCursor(0,0);
      lcd.print("Press middle to:");
      lcd.setCursor(0,2);
      lcd.print("  start game 3  ");

      if(activeren == HIGH)
      {
       Serial.print("4 actief");
       Serial.print("\n");
       lcd.setCursor(0,2);
       lcd.print("  game 3 actief ");

       //radio.begin(); //start de zender
       //radio.openWritingPipe(control1); //init waar de zender naartoe moet verzenden.
       radio.stopListening(); //door te stoppen met luisteren wordt het een zender.

       const char text[] = "3"; //maak een array met karakters genaamd text. Stop hierin "Hello World".
       radio.write(&text, sizeof(text)); //verstuur de data in de text.

       delay(1000);
       aantalDrukken = 1; //terug naar start Menu
       lcd.clear();
       activeren = LOW;
      }
      break;
     }
    laatsteMenu = aantalDrukken;
  }
}



void tmp()
{
  temp = analogRead(PT100);
  Serial.print(temp);
  Serial.print("\n");
  //Serial.println(PT100);
  //Serial.println("in2 ");
  //Serial.print(temp);
  // 0-5V komt binnen. 0= 0 graden en 5 = 100 graden. 5V is 1023.
  //om de waardes terug te brengen naar voltage moet er een berekening gedaan worden.
  float a = temp * (5 / 1023.0); //nu wordt de waarde van 1023 veranderd naar 0-100C
  temp = round(a);
 // Serial.println("Temp ");
  //Serial.print(temp);
  return temp;
}

String lght()
{
  int a = analogRead(LDR);
  //Serial.print(a);
 // Serial.print("\n");
  if (a <= 10) //ligt eraan hoe aangesloten. Kan ook boven de 1000 zijn.
  {
    light = "Night";
  }
  else
  {
    light = "Day";
  }
  return light;
}
