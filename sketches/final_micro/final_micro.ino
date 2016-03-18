//temp+pres
#include <Wire.h>
#include <qbcan.h>
#include <SPI.h>
BMP180 bmp;

void setup()
{
  //temp+pres
  bmp.begin();
}

void loop()
{
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
}
