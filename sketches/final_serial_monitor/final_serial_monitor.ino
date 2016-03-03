//card sd
#include <SD.h>
#include <SPI.h>
int CS_PIN = 10, aux=0;
File file;

//temp+humid
unsigned long previousMillis = 0;
#include <dht.h>
#define dht_apin A0
dht DHT;

//GPS
#include <SoftwareSerial.h>
SoftwareSerial gpsSerial(8, 9); // RX, TX (TX not used)
const int sentenceSize = 80;
char sentence[sentenceSize];

void setup()
{
  //card SD
  initializeSD();
  //speaker
  pinMode(6, OUTPUT);
  //GPS
  Serial.begin(9600);
  gpsSerial.begin(9600);
}

void loop()
{
  //sd
  //unsigned long millisSD = millis();
  //if(millisSD<=60000 && aux==0)
  //{
    createFile("datesenz.txt");
  //  if(aux==0)
  //  {
  //    aux=1;
  //    writeToFile("test ");
  //  }
  //  aux=1;
  //}
  //else
  //{
  //  if(millisSD<=120000&&aux==1)
  //  {
  //    closeFile();
  //    createFile("datesenz2.txt");
  //    aux=2;
  //  }
  //  else
  //  {
  //    if(millisSD>150000)
  //      closeFile();
  //  }
  //}
  
  //temp+humid
  unsigned long currentMillis = millis();
  //------------------------------GPS------------------------------
  int a, flag=0;
  static int i = 0;
  if (gpsSerial.available())
  {
    char ch = gpsSerial.read();
    if (ch != '\n' && i < sentenceSize)
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
      Serial.print("---------------GPS---------------\n");
      
      Serial.print("GMT: ");
      Serial.print(sentence[7]);
      Serial.print(sentence[8]);
      Serial.print(":");
      Serial.print(sentence[9]);
      Serial.print(sentence[10]);
      Serial.print(":");
      Serial.print(sentence[11]);
      Serial.print(sentence[12]);
      Serial.print("\n");

      writeToFile("GMT: ");
      writeToFileUnu(sentence[7]);
      writeToFileUnu(sentence[8]);
      writeToFile(":");
      writeToFileUnu(sentence[9]);
      writeToFileUnu(sentence[10]);
      writeToFile(":");
      writeToFileUnu(sentence[11]);
      writeToFileUnu(sentence[12]);
      writeToFile("\n");
      
      Serial.print("Lat: ");
      for(a=18; a<=26; a++)
        Serial.print(sentence[a]);
      Serial.print(" ");
      Serial.print(sentence[28]);
      Serial.print("\n");
      
      Serial.print("Long: ");
      for(a=30; a<=39; a++)
        Serial.print(sentence[a]);
      Serial.print(" ");
      Serial.print(sentence[41]);
      Serial.print("\n");

      Serial.print("Sat: ");
      Serial.print(sentence[45]);
      Serial.print(sentence[46]);
      Serial.print("\n");

      Serial.print("Alt: ");
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
              Serial.print(sentence[a]);
          }
        }
      }
      Serial.print(" m \n");
     }
    }
  }
  //------------------------------GPS------------------------------

  //temp+humid
  if(currentMillis-previousMillis>=2000) //minim 2 secunde intre citiri
  {
    previousMillis=currentMillis;
    DHT.read11(dht_apin);
    Serial.print("\nhum=");
    Serial.print(DHT.humidity);
    Serial.print("% temp=");
    Serial.print(DHT.temperature);
    Serial.print("C \n");
  }
  //temp+humid

  //speaker
  digitalWrite(6, HIGH);
  delay(1);
  digitalWrite(6,LOW);
  //speaker
  
  //sd
  closeFile();
} 


//--------------------CARD SD---------------------------------
void initializeSD()
{
  Serial.println("Initializing SD card...");
  pinMode(CS_PIN, OUTPUT);

  if (SD.begin())
  {
  } else
  {
    return;
  }
}

int createFile(char filename[])
{
  file = SD.open(filename, FILE_WRITE);

  if (file)
  {
    return 1;
  } else
  {
    return 0;
  }
}

int writeToFile(char text[])
{
  if (file)
  {
    file.println(text);
    return 1;
  } else
  {
    return 0;
  }
}

int writeToFileUnu(char text)
{
  if (file)
  {
    file.println(text);
    return 1;
  } else
  {
    return 0;
  }
}

int writeToFileDouble(double text)
{
  if (file)
  {
    file.println(text);
    return 1;
  } else
  {
    return 0;
  }
}

void closeFile()
{
  if (file)
  {
    file.close();
  }
}

int openFile(char filename[])
{
  file = SD.open(filename);
  if (file)
  {
    return 1;
  } else
  {
    return 0;
  }
}

String readLine()
{
  String received = "";
  char ch;
  while (file.available())
  {
    ch = file.read();
    if (ch == '\n')
    {
      return String(received);
    }
    else
    {
      received += ch;
    }
  }
  return "";
}

//--------------------CARD SD---------------------------------
