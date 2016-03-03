#include <qbcan.h>
#include <Wire.h>
#include <SPI.h>

BMP180 bmp;

void setup()
{
  Serial.begin(9600);
  Serial.println("REBOOT");

  if (bmp.begin())
    Serial.println("BMP180 init success");
  else
  {
    Serial.println("BMP180 init fail (disconnected?)\n\n");
    while(1); // Pause forever.
  }
}

void loop()
{
  double T,P;
  
  bmp.getData(T,P);

  Serial.print("Absolute pressure: ");
  Serial.print(P,2);
  Serial.println(" mb.");
  Serial.print("Temperature: ");
  Serial.print(T,2);
  Serial.println(" deg C.");

  delay(500);
}
