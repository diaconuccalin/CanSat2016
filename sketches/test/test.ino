#define SPKR 13 //this is the digital pin that you plugged the red       wire into

void setup()
{
   pinMode(SPKR, OUTPUT); //set the speaker as output
}

void loop() //do this over and over
{
   digitalWrite(SPKR, HIGH); //turn the speaker on
   delay(500);                     //wait for half a second
   digitalWrite(SPKR, LOW); //turn the speaker off
   delay(500);                    //wait for half a second
}
