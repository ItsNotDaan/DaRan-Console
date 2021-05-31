
//Dit programma is voor het veranderen van animaties in de tussentijd van het wachten.
unsigned long tijdTimer = 5000;
unsigned long anTijd = 1000;
unsigned long anHuidigeTijd = millis();
unsigned long huidigeTijd = millis(); //tijd hoelang het programma al draait. Long omdat het om tijd gaat
int i = 1;
while (millis() - huidigeTijd < tijdTimer) //doe 10 seconden lang mensen toevoegen die mee willen doen.
{
  Serial.println(millis());
  if(millis() - anHuidigeTijd > anTijd) // Elke seconden hierin.
  {
    anHuidigeTijd = millis();
    if(i < 5)
    {
      i++;
    }
    else {
      i = 1;
    }
  }
  Animatie_scherm(i,1);
}
