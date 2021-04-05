int aantal = 0;
char aangemeld[4];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  aangemeld[0] = '2';
  aangemeld[1] = '5';
  aangemeld[2] = '6';
  aangemeld[3] = '3';

  for (int i = 0; i < sizeof(aangemeld); i++)
  {
    if (aangemeld[i] != 0)
    {
       aantal++;
    }
  }
  //Serial.println(sizeof(aangemeld));
  Serial.println(aantal);
  aantal = 0;
  delay(1000);
}
