#include <qbcan.h>
#include <Wire.h>
#include <SPI.h>

BMP180 bmp;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  double T, P;
  bmp.getData(T, P);
  Serial.print("pressure:");
  Serial.print(P, 2);
  Serial.print("mb \n Temp(cansat sensor):");
  Serial.print(T,2);
  Serial.println("deg. C");
}
