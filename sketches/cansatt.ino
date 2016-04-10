#include <qbcan.h>
#include <Wire.h>
#include <SPI.h>

//Radio Parameters
#define NODEID        2    //unique for each node on same network
#define NETWORKID     2  //the same on all nodes that talk to each other
#define GATEWAYID     1    //Receiving node
#define ENCRYPTKEY    "sampleEncryptKey" //exactly the same 16 characters/bytes on all nodes!

//Radio object
char payload[50];
RFM69 radio;

void setup()
{
  //Initialize serial connection for debugging
  Serial.begin(9600);
  Serial.println("REBOOT");

  //  Initialize radio
  radio.initialize(FREQUENCY,NODEID,NETWORKID);
  radio.setHighPower(); //To use the high power capabilities of the RFM69HW
  radio.encrypt(ENCRYPTKEY);
  Serial.println("Transmitting at 433 Mhz");

}

void loop()
{
  sprintf(payload, "test");
  radio.send(GATEWAYID, payload, 15);
  Serial.println("Send complete");
  
  delay(500);
}
