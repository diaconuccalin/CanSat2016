//motoare
int motor_left[] = {6, 5};
int motor_right[] = {4, 1};

//geiger
#define CONV_FACTOR 0.00812

int geiger_input = 0;
long count = 0;
long countPerMinute = 0;
long timePrevious = 0;
long timePreviousMeassure = 0;
long time = 0;
long countPrevious = 0;
float radiationValue = 0.0;

//radio
#include <qbcan.h>
#include <SPI.h>
#include <Wire.h>

#define NODEID 2
#define NETWORKID 2
#define GATEWAYID 1
#define ENCRYPTKEY "sampleEncryptKey"

char payload[50];
RFM69 radio;

//sd
#include <SD.h>

//tempres
BMP180 bmp;

//temp+humid
unsigned long previousMillis = 0;
#include <dht.h>
#define dht_apin A0
dht DHT;

//GPS
#include <SoftwareSerial.h>
SoftwareSerial gpsSerial(9, 17); // RX, TX (TX not used)
char sentence[80];


void setup()
{
  //motoare
  int i;
  for(i = 0; i < 2; i++)
  {
    pinMode(motor_left[i], OUTPUT);
    pinMode(motor_right[i], OUTPUT);
  }
    
  //radio
  radio.initialize(FREQUENCY, NODEID, NETWORKID);
  radio.setHighPower();
  radio.encrypt(ENCRYPTKEY);

  //geiger
  pinMode(geiger_input, INPUT);
  digitalWrite(geiger_input,HIGH);
  
  attachInterrupt(digitalPinToInterrupt(0),countPulse,FALLING);
  
  //temp+pres
  bmp.begin();

  //speaker
  pinMode(19, OUTPUT);

  //GPS
  Serial.begin(9600);
  gpsSerial.begin(9600);

  //SD
  if(!SD.begin(8))
  {
    return;
  }
}

void loop()
{
  //SD
  File dataFile = SD.open("data.txt", FILE_WRITE);
  if(dataFile)
  {
  //SD
  
  //temp+humid
  unsigned long currentMillis = millis();

  //------------------------------GPS------------------------------
  int a, flag=0;
  static int i = 0;
  if (gpsSerial.available())
  {
    char ch = gpsSerial.read();
    if (ch != '\n' && i < 80)
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
      sprintf(payload, "GMT: ", sentence[7], sentence[8], ":", sentence[9], sentence[10], ":", sentence[11], sentence[12], "\n");
      dataFile.print(payload);
      radio.send(GATEWAYID, payload, 50);

      
      sprintf(payload, "Lat: ", sentence[18], sentence[19], sentence[20], sentence[21], sentence[22], sentence[23], sentence[24], sentence[25], sentence[26], " ", sentence[28], "\n");
      dataFile.print(payload);
      radio.send(GATEWAYID, payload, 50);
      
      
      sprintf(payload, "Long: ", sentence[30], sentence[31], sentence[32], sentence[33], sentence[34], sentence[35], sentence[36], sentence[37], sentence[38], sentence[39], " ", sentence[41], "\n");
      dataFile.print(payload);
      radio.send(GATEWAYID, payload, 50);
      

      sprintf(payload, "Sat: ", sentence[45], sentence[46], "\n");
      dataFile.print(payload);
      radio.send(GATEWAYID, payload, 50);
      
      int altitudine=0;
      for(a=48; a<=60 && flag!=2; a++)
      {
        if(flag==0)
        {
          if(sentence[a]==',')
            flag=1;
        }
        else
        {
          if(flag==1)
          {
            if(sentence[a]==',')
              flag=2;
            else
            {
              altitudine=altitudine*10+sentence[a]-'0';
            }
          }
        }
      }
      
      sprintf(payload, "Alt: ", altitudine, " m \n");
      radio.send(GATEWAYID, payload, 50);
      dataFile.print(payload);

      //motoare
      for (byte i = 0; i < radio.DATALEN; i++)
        if((char)radio.DATA[i]==0)
      //motoare
     }
    }
  }
  //------------------------------GPS------------------------------
  else
  {
    //temp+humid
    if(currentMillis-previousMillis>=2000) //minim 2 secunde intre citiri
    {     
      previousMillis=currentMillis;
      DHT.read11(dht_apin);

      sprintf(payload, "hum: ", DHT.humidity, "% \n");
      dataFile.print(payload);
      radio.send(GATEWAYID, payload, 50);


      sprintf(payload, "temp: ", DHT.temperature, "C \n");
      dataFile.print(payload);
      radio.send(GATEWAYID, payload, 50);
    }
    //temp+humid

    //temp+pres
    double T, P;
    bmp.getData(T,P);
    
    sprintf(payload,"T: %d C, P: %d mb.",(int)T,(int)P);
    dataFile.print(payload);
    radio.send(GATEWAYID, payload, 50);
    //temp+pres

    //geiger
    if (millis()-timePreviousMeassure > 10000)
    {
    countPerMinute = 6*count;
    radiationValue = countPerMinute * CONV_FACTOR;
    timePreviousMeassure = millis();
    sprintf(payload, "cpm = ", (countPerMinute, DEC), " - ", "uSv/h = ", (radiationValue, 4), "\n");
    dataFile.print(payload);
    radio.send(GATEWAYID, payload, 50);
    count = 0;
    }
 }

  //speaker
  digitalWrite(6, HIGH);
  delay(1);
  digitalWrite(6,LOW);
  //speaker

  //SD
  dataFile.close();
  }
  //SD

}

//geiger

void countPulse()
{
  detachInterrupt(digitalPinToInterrupt(0));
  count++;
  while(digitalRead(0)==0){}
  attachInterrupt(digitalPinToInterrupt(0),countPulse,FALLING);
}
