//temp+pres
#include <Wire.h>
#include <qbcan.h>
#include <SPI.h>
BMP180 bmp;

//DHT11
#include <dht.h>
#define dht_apin A0
dht DHT;
unsigned long previousMillis = 0;

//GPS
#include <SoftwareSerial.h>
SoftwareSerial gpsSerial(8, 9); //RX, TX (TX not used)
const int sentenceSize = 80;
char sentence[sentenceSize];

void setup()
{
  //temp+pres
  bmp.begin();
  
  //gps
  Serial.begin(9600);
  delay(1000);//delay to let the system boot
  gpsSerial.begin(9600);
  
  //speaker
   pinMode(6, OUTPUT);
}

void loop() //do this over and over
{
   //temp+umidit
   unsigned long currentMillis = millis();
   
   //GPS
   int a, flag=0;
   static int i=0;
   /*
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
   {
   */
    //DHT11(temp+umid)
    if(currentMillis-previousMillis>=2000)
    {
      previousMillis=currentMillis;
      DHT.read11(dht_apin);

      Serial.print("Current humidity = ");
      Serial.print(DHT.humidity);
      Serial.print("% temperature = ");
      Serial.print(DHT.temperature);
      Serial.println("C ");
    }
    //DHT11

    //temp+pres
    double T, P;
    bmp.getData(T, P);

    Serial.print("Pressure: ");
    Serial.print(P, 2);
    Serial.println(" mb.");
    Serial.print("Temperature: ");
    Serial.print(T, 2);
    Serial.println(" deg C");
    //temp+pres
   //}
   
   //speaker
   digitalWrite(6,HIGH);
   delay(1);
   digitalWrite(6, LOW);
}
