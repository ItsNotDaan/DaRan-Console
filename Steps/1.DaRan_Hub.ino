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
uint8_t adresHub[] = {0x00, 0xCE, 0xCC, 0xCE, 0xCC}; //Het adres voor de Hub
uint8_t adresCon[] = {0x01, 0xCE, 0xCC, 0xCE, 0xCC}; //Het adres voor alle controllers

// ----- Declare Objects -----
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //voor lcd
RF24 radio(9, 8);  // CE, CSN Dit is nodig voor de library om te kijken welke pinnen de zender is aangesloten.

// ----- Declare subroutines and/or functions -----


// ----- Declare Global Variables -----
int buzz = 23;
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
  pinMode(knopR, INPUT_PULLUP);
  pinMode(knopL, INPUT_PULLUP);
  pinMode(knopM, INPUT_PULLUP);
  pinMode(LDR, INPUT);
  pinMode(PT100, INPUT);
  pinMode(led, OUTPUT);
  pinMode(buzz, OUTPUT);

  Serial.begin(9600);

  radio.begin(); //Init the transceiver

  Serial.print("Radio aangesloten : ");
  Serial.println(radio.isChipConnected() ? "JA" : "NEE");

  radio.setPayloadSize(sizeof(t_message));
  radio.setPALevel(RF24_PA_LOW);

  radio.openWritingPipe(adresCon); //Schrijf altijd naar de controllers
  radio.openReadingPipe(1, adresHub); //Luister altijd naar de controllers
  radio.startListening(); //Start listening to signals.

  radio.startListening();

  lcd.begin(16,2);
  lcd.home();

  opstartProgramma();

}

//MAIN
void loop()
{
  if (digitalRead(knopR) == HIGH) {gedrukt = 1; isGedrukt = HIGH; tone(buzz,1000,100);}
  if (digitalRead(knopL) == HIGH) {gedrukt = 2; isGedrukt = HIGH; tone(buzz,1000,100);}
  if (digitalRead(knopM) == HIGH) {gedrukt = 3; isGedrukt = HIGH; tone(buzz,1000,100);}

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
      tmp();
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
      lcd.print(" Press to play: ");
      lcd.setCursor(0,2);
      lcd.print(" Fastest Presser");

      if(activeren == HIGH)
      {
        lcd.setCursor(0,0);
        lcd.print("Fastest Presser ");
        lcd.setCursor(0,2);
        lcd.print("     Active     ");

        bericht.command = '1'; //command 1 tells the cons that game 1 starts.
        stuurBericht(bericht); //Verstuur het bericht.
        bericht.ontvangerUID = 0;

        lcd.clear();
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
        lcd.print("      WAIT!     ");
        lcd.setCursor(0,2);
        lcd.print(" Timer started! ");



        //(11 - 10 = 1) < 10000 =  true
        //(12 - 10 = 2) < 10000 = true
        //.....
        //(10010 - 10 = 10000) =< 10000 = true
        //(10011 - 10 = 10001) =< 10000 = false dus doorgaan.
        bool verzonden = true;
        unsigned long tijdTimer = random(5000, 20000); //random tijd tussen de 5 en 20 seconden
        //long tijdTimer = 5000; //for testing
        unsigned long huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
        while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
        {
          if (radio.available()) //als er iets binnenkomt.
          {
            leesBericht(bericht);
            Serial.println(bericht.verzenderUID);
            Serial.print("heeft te snel gedrukt");

            bericht.command = 'F'; //F is van verloren
            bericht.ontvangerUID = bericht.verzenderUID;
            verzonden = false;
          }
        }

        lcd.setCursor(0,0);
        lcd.print("CLICK THE BUTTON");
        lcd.setCursor(0,2);
        lcd.print("      NOW!      ");
        bericht.ontvangerUID = 0;

        long reactieTijd;
        bool isWinnaar = false;
        tijdTimer = 10000; //Wacht 10 seconden. Als er niets is gedrukt dan gwn terug.
        huidigeTijd = millis(); //tijd hoelang het programma al draait.
        while (millis() - huidigeTijd < tijdTimer) //doe voor het aantal seconden alles wat in de while staat.
        {
          //animatie dat timer over is
          if(radio.available())
          {
            Serial.println("Winnaar is binnen");

            reactieTijd = millis() - huidigeTijd;
            isWinnaar = true;
            leesBericht(bericht);

            lcd.setCursor(0,0);
            lcd.print(" The winner is: ");
            lcd.setCursor(0,2);
            lcd.print("   Player:");
            lcd.setCursor(11,2);
            lcd.print(bericht.verzenderUID); //Show on the LCD who has won.

            bericht.ontvangerUID = bericht.verzenderUID;

            tijdTimer = 0; //Stop de timer
          }
        }

        bericht.command = '4'; //Put "4" in this char. //This will be send to all the controllers. They know they can go back to the beginning.
        bericht.alleCons = 1;
        stuurBericht(bericht);
        bericht.alleCons = 0;

        if (isWinnaar == true){
          delay(2000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(" Reaction time: ");
          lcd.setCursor(4,2);
          lcd.print(reactieTijd);
          lcd.setCursor(11,2);
          lcd.print("MS"); //Show on the LCD who has won.
          delay(2000);
        }
        else{
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("   Nobody has   ");
          lcd.setCursor(0,2);
          lcd.print("    clicked!    ");
          delay(4000);
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
      lcd.print(" Press to play: ");
      lcd.setCursor(0,2);
      lcd.print(" Roll the dice  ");

      if(activeren == HIGH)
      {
        lcd.setCursor(0,0);
        lcd.print("  Dobbelsteen   ");
        lcd.setCursor(0,2);
        lcd.print("     Active     ");

        bericht.command = '2'; //command 1 tells the cons that game 1 starts.
        stuurBericht(bericht); //Verstuur het bericht.

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  Press to add  ");
        lcd.setCursor(0,2);
        lcd.print("your controller ");

        //(11 - 10 = 1) < 10000 =  true
        //(12 - 10 = 2) < 10000 = true
        //.....
        //(10010 - 10 = 10000) =< 10000 = true
        //(10011 - 10 = 10001) =< 10000 = false dus doorgaan.
        int aangemeld[4]; //een array met 4 plekjes
        int spelers = 0; //Voor het bijvoegen van dingen in het array
        long tijdTimer = 10000; //10 seconden wachten.
        unsigned long huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
        while (millis() - huidigeTijd < tijdTimer) //doe 10 seconden lang mensen toevoegen die mee willen doen.
        {
         if (radio.available()) //als er iets binnenkomt.
         {
           Serial.print("Bericht binnen");
           leesBericht(bericht); //Lees het inkomende bericht
           aangemeld[spelers] = bericht.verzenderUID; //Elke keer als er iets binnenkomt dan wordt de waarde van de controller in de aangemelde array gegooid.
           spelers++;
           lcd.clear();
           lcd.setCursor(0,0);
           lcd.print("  Player: ");
           lcd.setCursor(11,0);
           lcd.print(bericht.verzenderUID);
           lcd.setCursor(0,2);
           lcd.print("     Added    ");

           delay(500);

           lcd.setCursor(0,0);
           lcd.print("  Press to add  ");
           lcd.setCursor(0,2);
           lcd.print("your controller ");
           //Doordat er bij de controller maar 1 keer gedrukt kan worden staat alles erin.
         }
        }

        if (spelers > 0) //Is er wel iemand die wil spelen?
        {
          int rondes = 0;
          int points[4] = {};
          int pointName[4] = {};
          while (spelers != rondes) //Rondes hetzelfde als het aantal mensen dat had gedrukt?
          {
            for (int a = 0; a < spelers; a++) //loop van 0 tot het aantal mensen dat heeft gedrukt.
            {
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print(" Controller: ");
              lcd.setCursor(14,0);
              lcd.print(aangemeld[a]);
              lcd.setCursor(0,2);
              lcd.print("   can throw    ");
              bericht.ontvangerUID = aangemeld[a]; //Geef door aan de controller dat die mag gooien

              stuurBericht(bericht); //Verstuur het bericht.

              bool klik = false;
              while (klik == false) //klik false?
              {
                if (radio.available()) //signaal binnen?
                {
                  //bool klaar = false;
                  leesBericht(bericht);
                  if (bericht.verzenderUID == aangemeld[a]) //Zelfde als degene die als eerste mocht gooien?
                  {
                    int gooi = random(1,6); //maak een getal tussen de 1 en 6.
                    points[a] = gooi; //De waarde van gooi in array points.
                    pointName[a] = bericht.verzenderUID; //De controller naam van de gooier op dezelfde plek als de hoogste.
                    klik = true;
                  }
                }
              }
              rondes++; //Langs alle spelers gaan.
              Serial.println(rondes);
            }
          }

          int aantal1, aantal2, aantal;
          aantal1 = max(points[0], points[1]); //Max van deze twee in aantal1;
          aantal2 = max(points[2], points[3]); //Max van deze twee in aantal2;
          aantal = max(aantal1, aantal2); //max van de 4 in aantal.
          Serial.println(aantal);
          int winner/*[4] = {}*/;
          int b;
          for (int a = 0; a < 4; a++) //ga voor 4 keer kijken welk nummer het hoogste is.
          {
            /*if (points[a] == winner) //is er nog een keer de waarde van de winnaar?
            {
              b++
            }*/

            if (points[a] == aantal) //komt de waarde overeen met het getal in het array.
            {
              winner/*[b]*/ = a; //de waarde van a in winnaar stoppen.
            }
            //winner kan nu zijn {2,4,0,0} Controller 2 en 4 zijn dus dubbel en moeten nog een keer.
          }
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("    Who has    ");
          lcd.setCursor(0,2);
          lcd.print("    Won????    ");
          delay(2000);
          /*
          De naam van de winnaar wordt uit het array gehaald.
          pointName[0,3,2,0]
          points[0,1,6,0]
          Controller 2 is de winnaar met 6 punten.
          */

          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Controller   won");
          lcd.setCursor(11,0);
          lcd.print(pointName[winner]);
          lcd.setCursor(0,2);
          lcd.print(" With   Points ");
          lcd.setCursor(6,2);
          lcd.print(points[winner]);

          bericht.ontvangerUID = pointName[winner]; //de Winnaar wordt gestopt in ontvangerUID.
        }
        else {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("  Nobody wants  ");
          lcd.setCursor(0,2);
          lcd.print("    to play!    ");
          bericht.ontvangerUID = 0;
        }

        bericht.command = '4';
        bericht.alleCons = 1; //Alle controllers moeten dit commando aannemen.
        stuurBericht(bericht); //Verstuur het bericht.
        bericht.alleCons = 0; //Zet alle controllers uit.

        delay(4000);
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
      lcd.print(" Press to play: ");
      lcd.setCursor(0,2);
      lcd.print(" Clicking bomb  ");

      if(activeren == HIGH)
      {
         lcd.setCursor(0,0);
         lcd.print("Press until dead");
         lcd.setCursor(0,2);
         lcd.print("     Active     ");

         bericht.command = '3'; //command 1 tells the cons that game 3 starts.
         stuurBericht(bericht); //Verstuur het bericht.
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("  Press to add  ");
         lcd.setCursor(0,2);
         lcd.print("your controller ");

         int aangemeld[4]; //een array met 4 plekjes
         int a = 0; //Voor het bijvoegen van dingen in het array
         long tijdTimer = 10000; //10 seconden wachten.
         unsigned long huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
         while (millis() - huidigeTijd < tijdTimer) //doe 10 seconden lang mensen toevoegen die mee willen doen.
         {
            if (radio.available()) //als er iets binnenkomt.
            {
              Serial.println("Bericht binnen");
              leesBericht(bericht); //Lees het inkomende bericht
              aangemeld[a] = bericht.verzenderUID; //Elke keer als er iets binnenkomt dan wordt de waarde van de controller in de aangemelde array gegooid.
              a++;
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("  Player: ");
              lcd.setCursor(11,0);
              lcd.print(bericht.verzenderUID);
              lcd.setCursor(0,2);
              lcd.print("     Added    ");

              delay(500);

              lcd.setCursor(0,0);
              lcd.print("  Press to add  ");
              lcd.setCursor(0,2);
              lcd.print("your controller ");
              //Doordat er bij de controller maar 1 keer gedrukt kan worden staat alles erin.
            }
         }



         int rondes = random(1, 21); //Maak een random getal tussen de 1 en 20
         int rondesKlaar = 0;
         int spelersCheck = 0;
         int spelers = a;
         int pointName[4] = {};


         if (spelers > 0) //Zijn er wel spelers??
         {
           while (rondesKlaar != rondes) //Rondes hetzelfde als het aantal mensen dat had gedrukt?
           {
            Serial.print("rondesKlaar = ");
            Serial.println(rondesKlaar);
            Serial.print("spelersCheck = ");
            Serial.println(spelersCheck);
            Serial.print("spelers = ");
            Serial.println(spelers);
             if (spelers == spelersCheck) //Check of de spelers zijn geweest.
             {
               /*
               1,2,3,4 -> 1,2,3  spelersCheck
               1,2,3,4    5,6,7 verloren!
               */
               spelersCheck = 0;
             }
             bool klik = false;
             lcd.clear();
             lcd.setCursor(0,0);
             lcd.print(" Controller: ");
             lcd.setCursor(14,0);
             lcd.print(aangemeld[spelersCheck]);
             lcd.setCursor(0,2);
             lcd.print("  can click  ");
             bericht.ontvangerUID = aangemeld[spelersCheck]; //Geef door aan de controller dat die mag gooien
             stuurBericht(bericht);

             while(klik == false)
             {
               if (radio.available()) //signaal binnen?
               {
                 Serial.println("SIGNAAL BINNEN");
                 leesBericht(bericht);
                 if (bericht.verzenderUID == aangemeld[spelersCheck]) //Zelfde als degene die als eerste mocht gooien?
                 {
                   Serial.println("CONTROLLER HEEFT GEGOOID");
                   lcd.clear();
                   lcd.setCursor(0,0);
                   lcd.print(" Controller: ");
                   lcd.setCursor(14,0);
                   lcd.print(aangemeld[spelersCheck]);
                   lcd.setCursor(0,2);
                   lcd.print(" has clicked ");

                   spelersCheck++;
                   rondesKlaar++;
                   delay(500);

                   klik = true;
                 }
               }
             }
           }
           spelersCheck--; //Er moet altijd 1 af want als laatste wordt er 1 gegooid.

           bericht.ontvangerUID = aangemeld[spelersCheck]; //de Winnaar wordt gestopt in ontvangerUID.
           Serial.print("Controller die wint: ");
           Serial.println(aangemeld[spelersCheck]);
           bericht.command = '4';
           bericht.alleCons = 1; //Alle controllers moeten dit commando aannemen.
           stuurBericht(bericht); //Verstuur het bericht.
           bericht.alleCons = 0; //Zet alle controllers uit.

           lcd.clear();
           lcd.setCursor(0,0);
           lcd.print(" Controller: ");
           lcd.setCursor(14,0);
           lcd.print(aangemeld[spelersCheck]);
           lcd.setCursor(0,2);
           lcd.print("     LOST     ");
           delay(2000);

           lcd.clear();
           lcd.setCursor(0,0);
           lcd.print("    With:       ");
           lcd.setCursor(10,0);
           lcd.print(rondes);
           lcd.setCursor(0,2);
           lcd.print("     rounds!    ");
           delay(2000);
         }

         else { //Niemand gegooid?
           bericht.ontvangerUID = 0;
           bericht.command = '4';
           bericht.alleCons = 1; //Alle controllers moeten dit commando aannemen.
           stuurBericht(bericht); //Verstuur het bericht.
           bericht.alleCons = 0; //Zet alle controllers uit.

           lcd.clear();
           lcd.setCursor(0,0);
           lcd.print("  Nobody wants  ");
           lcd.setCursor(0,2);
           lcd.print("    to play!    ");

           delay (4000);
         }

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
  Serial.println(temp);
  // 0-5V komt binnen. 0= 0 graden en 5 = 100 graden. 5V is 1023.
  //om de waardes terug te brengen naar voltage moet er een berekening gedaan worden.
  int a = temp * (50 / 1023.0); //nu wordt de waarde van 1023 veranderd naar 0-100C
  temp = round(a);
  //temp = 19;
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

void opstartProgramma(){
    lght();
    if (light == "Day") {
      lcd.setCursor(0,2);
      lcd.print("  Goodmorning   ");
    }
    else if (light == "Night") {
      lcd.setCursor(0,2);
      lcd.print("   Goodnight    ");
    }
    else {
      lcd.setCursor(0,2);
      lcd.print("  Daan & Emran  ");
    }

    lcd.setCursor(0,0);
    lcd.print(" DaRan Console  ");

    int a = 500;
    for(int i = 0; i < 5; i++)
    {
      tone(buzz,a,150);
      delay(150);
      a = a + 100;
    }
    tone(buzz,1000,400);
    delay(600);
    tone(buzz,1000,150);
    delay(250);
    tone(buzz,1000,150);
    delay(200);
    tone(buzz,1000,600);
    delay(600);
    noTone(buzz);
    lcd.clear();
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
