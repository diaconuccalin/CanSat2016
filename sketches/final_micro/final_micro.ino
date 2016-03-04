//GPS
#include <SoftwareSerial.h>
SoftwareSerial gpsSerial(8, 9); //RX, TX (TX not used)
const int sentenceSize = 80;
char sentence[sentenceSize];

void setup()
{
  //gps
  Serial.begin(9600);
  gpsSerial.begin(9600);
  //spekaer
   pinMode(6, OUTPUT);
  //curent
   pinMode(7,OUTPUT);
   digitalWrite(7, HIGH);
}

void loop() //do this over and over
{
   //speaker
   digitalWrite(6, HIGH);
   //GPS
   int a, flag=0;
   static int i=0;
   if(gpsSerial.available())
   {
    char ch = gpsSerial.read();
    if(ch != '\n' && i < sentenceSize)
    {
      sentence[i] = ch;
      i++;
    }
    else
    {
      sentence[i] = '\0';
      i = 0;

      if(sentence[4]=='G')
      {
        Serial.println("\n \n \n--------------------------GPS-----------------------------");

        Serial.print("GMT: ");
        Serial.print(sentence[7]);
        Serial.print(sentence[8]);
        Serial.print(":");
        Serial.print(sentence[9]);
        Serial.print(sentence[10]);
        Serial.print(":");
        Serial.print(sentence[11]);
        Serial.println(sentence[12]);

        Serial.print("Lat: ");
        for(a=18; a<=26; a++)
          Serial.print(sentence[a]);
        Serial.print(" ");
        Serial.println(sentence[28]);

        Serial.print("Long: ");
        for(a=30; a<=39; a++)
          Serial.print(sentence[a]);
        Serial.print(" ");
        Serial.println(sentence[41]);

        Serial.print("Sat: ");
        Serial.print(sentence[45]);
        Serial.println(sentence[46]);

        Serial.print("Alt: ");
        for(a=48; a<=60 && flag!=2; a++)
        {
          if(flag==0)
          {
            if(sentence[a]==',')
              flag=2;
            else
              Serial.print(sentence[a]);
          }
        }
        Serial.println(" m");
      }
    }
   }
   //gps
   else
   {}
   //speker
   digitalWrite(6, LOW);
}
